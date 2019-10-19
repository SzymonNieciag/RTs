// Copyright 2019 Dan Kestranek.


#include "GDPlayerState.h"
#include "GDPlayerController.h"
#include "Abilities/TSAbilitySystemComponent.h"
#include "Abilities/AttributeSetBase.h"
#include "Characters/Heroes/TDSPlayerCharacter.h"

AGDPlayerState::AGDPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UTSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent * AGDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSetBase * AGDPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool AGDPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AGDPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AGDPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

int32 AGDPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

int32 AGDPlayerState::GetXP() const
{
	return AttributeSetBase->GetXP();
}


void AGDPlayerState::BeginPlay()
{
		Super::BeginPlay();

		if (AbilitySystemComponent)
		{
			// Attribute change callbacks
			HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGDPlayerState::HealthChanged);
			// Tag change callbacks
			// AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGDPlayerState::StunTagChanged);
		}
	

}

void AGDPlayerState::HealthChanged(const FOnAttributeChangeData & Data)
{
	float Health = Data.NewValue;

	// Update floating status bar
	ATDSPlayerCharacter* Hero = Cast<ATDSPlayerCharacter>(GetPawn());
	/*if (Hero)
	{
		UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
		}
	}*/

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

	// If the player died, handle death
	if (!IsAlive())
	{
		if (Hero)
		{
			Hero->Die();
		}
	}
}
