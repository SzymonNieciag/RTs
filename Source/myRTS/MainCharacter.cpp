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

// Sets default values
AMainCharacter::AMainCharacter()
	:CoverActor(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTreeReference"));

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MainMesh->SetupAttachment(RootComponent);
	MainMesh->RelativeLocation = FVector(0,0,-90);

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
	MovementPtr->MaxWalkSpeed = Speed;

	AttributeSetBase->OnHealtChange.AddDynamic(this, &AMainCharacter::OnHealtChanged);
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

void AMainCharacter::Destroyed()
{
	Super::Destroyed();

	if (UnitsSquad != NULL)
	{
		UnitsSquad->RemoveFromSquad(this);
		// Remove this actor from GameMode Pawn TArray
		AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
		RTSGameMode->AllPawns.Remove(this);
	}
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

void AMainCharacter::OnHealtChanged(float Health, float MaxHealth)
{
	if (Health <= 0)
	{
		BP_Die();
		return;
	}
	BP_OnHealtChanged(Health, MaxHealth);
}

float AMainCharacter::TakeRTSDamage_Implementation(float DamageAmount, AActor* DamageCauser)
{
	float ActualDamage = DamageAmount;

	if (ActualDamage != 0.f)
	{
		ReceiveRTSDamage(ActualDamage, DamageCauser);
		OnTakeRTSDamage.Broadcast(ActualDamage, DamageCauser);
	}
	return ActualDamage;
}

void AMainCharacter::ReceiveRTSDamage(float DamageAmount, AActor* DamageCauser)
{
	Healt -= DamageAmount;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("zycie = %f"), Healt));

	if (Healt <= 0)
	{
		Destroy();
	}
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