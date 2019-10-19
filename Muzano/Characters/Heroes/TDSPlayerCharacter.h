// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GDCharacterBase.h"
#include "TDSPlayerCharacter.generated.h"

UCLASS(config=Game)
class ATDSPlayerCharacter : public AGDCharacterBase
{
	GENERATED_BODY()

public:

	ATDSPlayerCharacter(const class FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GASDocumentation|Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GASDocumentation|Camera")
	class UCameraComponent* FollowCamera;

	// Client only
	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration);
	/**
	* Called when character takes damage, which may have killed them
	*
	* @param DamageAmount Amount of damage that was done, not clamped based on current health
	* @param HitInfo The hit info that generated this damage
	* @param DamageTags The gameplay tags of the event that did the damage
	* @param InstigatorCharacter The character that initiated this damage
	* @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ATDSPlayerCharacter* InstigatorCharacter, AActor* DamageCauser);
	/**
	* Called when health is changed, either from healing or from being damaged
	* For damage this is called in addition to OnDamaged/OnKilled
	*
	* @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	* @param EventTags The gameplay tags of the event that changed mana
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
	void OnHealtChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnDeath"))
	void OnDeath();

	// Called from RPGAttributeSet, these call BP events above
	//void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ATDSPlayerCharacter* InstigatorPawn, AActor* DamageCauser);
	//void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	//void HandleDeath();

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
