// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanBeDamaged.generated.h"

// This class does not need to be modified.
UENUM(BlueprintType)
enum class EDamageType :uint8
{
	Physic = 0 UMETA(DisplayName = "Physic"),
};
UENUM(BlueprintType)
enum class EStateType :uint8
{
	Walk = 0 UMETA(DisplayName = "WalkState"),
	Attack = 1 UMETA(DisplayName = "AttackState"),
	Idle = 2 UMETA(DisplayName = "idleState"),
};
UINTERFACE(MinimalAPI)
class UCanBeDamaged : public UInterface
{
	GENERATED_BODY()
};
class MYRTS_API ICanBeDamaged
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
		void SetDamage(float Damage, AActor* DamageCauser);
};