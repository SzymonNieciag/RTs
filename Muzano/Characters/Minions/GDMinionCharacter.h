// Copyright 2019 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GDCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "GDMinionCharacter.generated.h"

/**
 * An AI controlled minion character.
 */
UCLASS()
class MUZANO_API AGDMinionCharacter : public AGDCharacterBase
{
	GENERATED_BODY()

public:
	AGDMinionCharacter(const class FObjectInitializer& ObjectInitializer);

protected:

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY()
	class UTSAbilitySystemComponent* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY()
	class UAttributeSetBase* HardRefAttributeSetBase;
	
	virtual void BeginPlay() override;
};
