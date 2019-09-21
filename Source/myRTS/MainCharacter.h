// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CanBeDamaged.h"
#include <BehaviorTree/BehaviorTree.h>
#include "Interface/IsSelectable.h"
#include <../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h>
#include <../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h>
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTakeRTSDamageSignature, float, DamageAmount, AActor*, DamageCauser);

class UAttributeSetBase;
class ACoverActorBase;

UCLASS()
class MYRTS_API AMainCharacter : public ACharacter, public ICanBeDamaged, public IIsSelectable, public IAbilitySystemInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent *MainMesh;

	//************//  Ability System //************//

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		UAbilitySystemComponent *AbilitySystemComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		UAttributeSetBase *AttributeSetBase;
	UFUNCTION(BlueprintCallable, Category = "CharacterMesh")
		void AquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration);

	UFUNCTION()
		void OnHealtChanged(float Health, float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
		void BP_OnHealtChanged(float Health, float MaxHealth);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
		void BP_Die();

	////////////////////////////////////////////

	//****************//  AI //***************//

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree *BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Squad")
		class AUnitsSquad *UnitsSquad;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
		ACoverActorBase *CoverActor;
	//////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Stat")
		float Healt = 100;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float Attack = 10;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float Armor = 10;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float RangeAttack = 200;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float Speed = 400;
	UPROPERTY(EditAnywhere, Category = "Stat")
		bool isAlive = true;
	UPROPERTY(EditAnywhere, Category = "Navigation")
		float NavLocationSize = 50;
	
	UPROPERTY(BlueprintAssignable, Category = "RTS|Damage")
		FTakeRTSDamageSignature OnTakeRTSDamage;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "RTS|Damage")
		float TakeRTSDamage(float DamageAmount, AActor* DamageCauser);
	virtual float TakeRTSDamage_Implementation(float DamageAmount, AActor* DamageCauser) override;

	virtual void ReceiveRTSDamage(float DamageAmount, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Decal")
		void EnableDecalEffect();  
	virtual void EnableDecalEffect_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Decal")
		void DisableDecalEffect();
	virtual void DisableDecalEffect_Implementation() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void Destroyed() override;

	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
};




//
//bool operator!=(const FGameplayAbilitySpecHandle& Other) const
//{
//	return Handle != Other.Handle;
//}