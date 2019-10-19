// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"
#include <GameplayEffectExtension.h>
#include <GameplayEffect.h>
#include <UnrealMathUtility.h>
#include <UnrealNetwork.h>
#include "Characters/GDCharacterBase.h"


UAttributeSetBase::UAttributeSetBase()
	: Health(100.f)
	, MaxHealth(100.f)
	,AttackPower(1.0f)
{

}

void UAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributeSetBase, Health);
	DOREPLIFETIME(UAttributeSetBase, MaxHealth);
	DOREPLIFETIME(UAttributeSetBase, AttackPower);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, XP, COND_None, REPNOTIFY_Always);
}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue)
{
	/*if (Attribute == GetAttackPowerAttribute())
	{

	}*/
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AGDCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AGDCharacterBase>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		AGDCharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}
			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<AGDCharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<AGDCharacterBase>(SourceActor);
			}
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("damage = %f"), LocalDamageDone));

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}


	//else if (Data.EvaluatedData.Attribute == GetStatminaAttribute())
	//{
	//	// Clamp mana
	//	SetStatmina(FMath::Clamp(GetStatmina(), 0.0f, GetMaxStamina()));

	//	if (TargetCharacter)
	//	{
	//		// Call for all mana changes
	//		TargetCharacter->handle(DeltaValue, SourceTags);
	//	}
	//}
}

void UAttributeSetBase::OnRep_Health()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Health);
}

void UAttributeSetBase::OnRep_MaxHealth()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxHealth);
}

void UAttributeSetBase::OnRep_AttackPower()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, AttackPower);
}

void UAttributeSetBase::OnRep_CharacterLevel()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, CharacterLevel);
}

void UAttributeSetBase::OnRep_XP()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, XP);
}