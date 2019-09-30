// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include "UnitsSquad.h"
#include "myRTSGameMode.h"
#include <../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h>
#include "Abilities/AttributeSetBase.h"
#include "CoverActorBase.h"
#include <AIController.h>

// Sets default values
AMainCharacter::AMainCharacter()
	:CoverActor(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTreeReference"));

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("UAttributeSetBaseComp"));
	AttributeSetBase->Health = 200;
	AttributeSetBase->MaxHealth = 200;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent *MovementPtr = Cast<UCharacterMovementComponent>(GetCharacterMovement());

	//Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//AttributeSetBase->OnHealtChange.AddDynamic(this, &AMainCharacter::HandleHealthChanged);
}

bool AMainCharacter::IsAlive()
{
	if (GetHealth()>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AMainCharacter::EnableDecalEffect_Implementation()
{

}

void AMainCharacter::DisableDecalEffect_Implementation()
{

}

UAbilitySystemComponent * AMainCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
	if (AbilitySystemComponent)
	{
		if (HasAuthority() && AbilityToAquire)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToAquire, 1, 0));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AMainCharacter::HandleDamage(float DamageAmount, const FHitResult & HitInfo, const FGameplayTagContainer & DamageTags, AMainCharacter * InstigatorPawn, AActor * DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);
}

void AMainCharacter::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	OnHealtChanged(DeltaValue, EventTags);

	if (!IsAlive())
	{
		AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
		RTSGameMode->AllPawns.Remove(this);
		OnDeath();
	}
}

float AMainCharacter::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AMainCharacter::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AMainCharacter::GetStamina() const
{
	return AttributeSetBase->GetStatmina();
}

float AMainCharacter::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float AMainCharacter::CalculateChanceToHitTarget(AActor *TargetActor)
{
	if (Weapon)
	{
		AMainCharacter *TargetMainCharacter = Cast<AMainCharacter>(TargetActor);
		//if (!TargetMainCharacter)
		//	return 0.0f;
		ACoverActorBase *TargetCoverActorBase = TargetMainCharacter->CoverActor;

		if (TargetCoverActorBase) 
		{
			return TargetCoverActorBase->CoverData.ReduceChanceToHit * Weapon->CalculateWeaponAccuracyAtDistance(AActor::GetDistanceTo(TargetActor));;
		}
		else
		{
			Weapon->CalculateWeaponAccuracyAtDistance(AActor::GetDistanceTo(TargetActor));;
		}
	}
	return 0.0f;
}

bool AMainCharacter::IsCovered()
{
	if (CoverActor)
	{
		if (GetSquaredDistanceTo(CoverActor) < SafeSqrDistance)
		{
			true;
		}
		else
		{
			false;
		}
	}
	return false;
}

bool AMainCharacter::LeaveTheCover()
{
	if (CoverActor)
	{
		CoverActor->SetDestinateTargetActor(nullptr);
		CoverActor = nullptr;
		return true;
	}
	return false;
}

bool AMainCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}

void AMainCharacter::Destroyed()
{
	Super::Destroyed();

	if (UnitsSquad != NULL)
	{
		UnitsSquad->RemoveFromSquad(this);
		// Remove this actor from GameMode Pawn TArray
	}
}
