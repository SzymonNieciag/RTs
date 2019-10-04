// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CanBeDamaged.h"
#include <BehaviorTree/BehaviorTree.h>
#include "Interface/IsSelectable.h"
#include <../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h>
#include <../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h>
#include "Item/Weapon.h"
#include "MainCharacter.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTakeRTSDamageSignature, float, DeltaValue, const struct FGameplayTagContainer&, EventTags);

class UAttributeSetBase;
class ACoverGoalPoint;

#define SafeSqrDistance 10000.0f

UCLASS()
class MYRTS_API AMainCharacter : public ACharacter, public IIsSelectable, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMainCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//************//  Ability System //************//

	/** The component used to handle ability system interactions */
	UPROPERTY(BlueprintReadOnly, Category = "CharacterBase")
		UAbilitySystemComponent *AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY(BlueprintReadOnly, Category = "CharacterBase")
		UAttributeSetBase *AttributeSetBase;

	UFUNCTION(BlueprintCallable, Category = "CharacterMesh")
		void AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);

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
		void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AMainCharacter* InstigatorCharacter, AActor* DamageCauser);
	/**
	* Called when health is changed, either from healing or from being damaged
	* For damage this is called in addition to OnDamaged/OnKilled
	*
	* @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	* @param EventTags The gameplay tags of the event that changed mana
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
		void OnHealtChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Called from RPGAttributeSet, these call BP events above
		void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AMainCharacter* InstigatorPawn, AActor* DamageCauser);
		void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
		void HandleDeath();

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "Character"))
		void OnDeath();

	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable, Category = "Damage")
		virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable, Category = "Damage")
		virtual float GetMaxHealth() const;

	/** Returns current stamina */
	UFUNCTION(BlueprintCallable, Category = "Damage")
		virtual float GetStamina() const;

	/** Returns maximum stamina, stamina will never be greater than this */
	UFUNCTION(BlueprintCallable, Category = "Damage")
		virtual float GetMaxStamina() const;

	//****************//  AI //***************//

	UPROPERTY(BlueprintReadOnly, Category = "AI")
		UBehaviorTree *BehaviorTree;

	UPROPERTY(BlueprintReadOnly, Category = "Squad")
		class AUnitsSquad *UnitsSquad;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
		class ACoverGoalPoint *CoverPoint;

	/* Return true if is covered */
	UFUNCTION(BlueprintCallable, Category = "AI")
		bool IsCovered();
		bool CheckCoverPoint(ACoverGoalPoint* CoverGoalPoint);

	/* Return true if was covered */
	UFUNCTION(BlueprintCallable, Category = "AI")
		bool LeaveTheCover();
	

	FORCEINLINE class ACoverGoalPoint* GetCoveredActor() { return CoverPoint; }
	//////////////////////////////////////////////////

	//****************//  Stats //***************//

	/* Check Weapon is AttachementToActor on BeginPlay in BP_Character */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Items|Weapon")
		AWeapon *Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
		float NavLocationSize = 100;

	UFUNCTION(BlueprintCallable, Category = "AI")
		bool IsAlive();

	/* Calculation of the total accuracy allowing to hit the target */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Damage")
		float CalculateChanceToHitTarget(AActor *TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Decal")
		void EnableDecalEffect();  
	virtual void EnableDecalEffect_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Decal")
		void DisableDecalEffect();
	virtual void DisableDecalEffect_Implementation() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void Destroyed() override;
};


//
//bool operator!=(const FGameplayAbilitySpecHandle& Other) const
//{
//	return Handle != Other.Handle;
//}