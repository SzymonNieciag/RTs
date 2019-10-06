// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSCharacter.h"
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
ARTSCharacter::ARTSCharacter()
	:CoverPoint(nullptr), IsDead(false)
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
void ARTSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent *MovementPtr = Cast<UCharacterMovementComponent>(GetCharacterMovement());

	//Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//AttributeSetBase->OnHealtChange.AddDynamic(this, &AMainCharacter::HandleHealthChanged);
}

bool ARTSCharacter::IsAlive()
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

void ARTSCharacter::EnableSelectedDecal_Implementation()
{

}

void ARTSCharacter::DisableSelectedDecal_Implementation()
{

}

void ARTSCharacter::ReTriggerPreviewDecal_Implementation()
{
}

UAbilitySystemComponent * ARTSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARTSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARTSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARTSCharacter::AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
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

void ARTSCharacter::HandleDamage(float DamageAmount, const FHitResult & HitInfo, const FGameplayTagContainer & DamageTags, ARTSCharacter * InstigatorPawn, AActor * DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);
}

void ARTSCharacter::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	OnHealtChanged(DeltaValue, EventTags);

	if (!IsAlive() && !IsDead)
	{
		IsDead = true;
		AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
		RTSGameMode->AllPawns.Remove(this);
		HandleDeath();
	}
}

void ARTSCharacter::HandleDeath()
{
	LeaveTheCover();
	OnDeath();
}

float ARTSCharacter::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ARTSCharacter::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ARTSCharacter::GetStamina() const
{
	return AttributeSetBase->GetStatmina();
}

float ARTSCharacter::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float ARTSCharacter::CalculateChanceToHitTarget(AActor *TargetActor)
{
	if (Weapon)
	{
		ARTSCharacter *TargetMainCharacter = Cast<ARTSCharacter>(TargetActor);
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

			if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, COLLISION_PROJECTILE, CollisionParams))
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The Component Being Hit is: %s"), *OutHit.GetComponent()->GetName())); //dzia.³a
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

bool ARTSCharacter::IsCovered()
{
	return CheckCoverPoint(CoverPoint);
}

bool ARTSCharacter::CheckCoverPoint(ACoverGoalPoint* CoverGoalPoint)
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

bool ARTSCharacter::LeaveTheCover()
{
	if (CoverPoint)
	{
		CoverPoint->SetDestinateTargetActor(nullptr);
		CoverPoint = nullptr;
		return true;
	}
	return false;
}

bool ARTSCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
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

void ARTSCharacter::Destroyed()
{
	Super::Destroyed();

	if (UnitsSquad != NULL)
	{
		UnitsSquad->RemoveFromSquad(this);
		// Remove this actor from GameMode Pawn TArray
	}
}
