// Copyright 2019 Dan Kestranek.


#include "GDCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GDCharacterMovementComponent.h"
#include "Abilities/TSAbilitySystemComponent.h"
#include "Abilities/AttributeSetBase.h"
#include "Abilities/GDGameplayAbility.h"

// Sets default values
AGDCharacterBase::AGDCharacterBase(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGDCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AGDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent * AGDCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AGDCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AGDCharacterBase::GetAbilityLevel(EAbilityInputID AbilityID) const
{
	return 1;
}

void AGDCharacterBase::RemoveCharacterAbilities()
{
	if (Role != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = false;
}

float AGDCharacterBase::GetHealth() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float AGDCharacterBase::GetMaxHealth() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

int32 AGDCharacterBase::GetCharacterLevel() const
{
	if (AttributeSetBase)
	{
		return static_cast<int32>(AttributeSetBase->GetCharacterLevel());
	}

	return 0.0f;
}
// Run on Server and all clients
void AGDCharacterBase::Die()
{
	// Only runs on Server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
	}
}

void AGDCharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (Role != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UGDGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}

void AGDCharacterBase::InitializeAttributes()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), TEXT(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void AGDCharacterBase::AddStartupEffects()
{
	if (Role != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void AGDCharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase)
	{
		AttributeSetBase->SetHealth(Health);
	}
}

void AGDCharacterBase::HandleDamage(const float LocalDamageDone, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AGDCharacterBase* SourceCharacter, AActor* SourceActor)
{
	OnHandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);
}
