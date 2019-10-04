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
#include <AIController.h>
#include "Environment/CoverGoalPoint.h"
#include <DrawDebugHelpers.h>

// Sets default values
AMainCharacter::AMainCharacter()
	:CoverPoint(nullptr)
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
		HandleDeath();
	}
}

void AMainCharacter::HandleDeath()
{
	LeaveTheCover();
	OnDeath();
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
		ACoverGoalPoint *TargetCoverPoint = TargetMainCharacter->CoverPoint;

		if (TargetCoverPoint) 
		{
			FHitResult OutHit;
			FVector Start = TargetMainCharacter->GetActorLocation();

			Start.Z += 50.f;

			FVector Direction = this->GetActorLocation() - TargetMainCharacter->GetActorLocation();
			Direction.Normalize(0.1);
			FVector End = ((Direction * 300.f) + Start);

			FCollisionQueryParams CollisionParams;

			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 5);

			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_EngineTraceChannel1, CollisionParams))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *OutHit.GetComponent()->GetName()));
			}
			DrawDebugSphere(GetWorld(), TargetCoverPoint->GetActorLocation(), 160, 3, FColor::Blue, false, 3.0f, 25.0f);
			return TargetCoverPoint->CoverData.ReduceChanceToHit * Weapon->CalculateWeaponAccuracyAtDistance(this->GetDistanceTo(TargetActor));
		}
		else
		{
			return Weapon->CalculateWeaponAccuracyAtDistance(this->GetDistanceTo(TargetActor));;
		}
	}
	return 0.0f;
}

bool AMainCharacter::IsCovered()
{
	return CheckCoverPoint(CoverPoint);
}

bool AMainCharacter::CheckCoverPoint(ACoverGoalPoint* CoverGoalPoint)
{
	if (CoverGoalPoint)
	{
		if (GetSquaredDistanceTo(CoverGoalPoint) < SafeSqrDistance)
		{
			 return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool AMainCharacter::LeaveTheCover()
{
	if (CoverPoint)
	{
		CoverPoint->SetDestinateTargetActor(nullptr);
		CoverPoint = nullptr;
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
