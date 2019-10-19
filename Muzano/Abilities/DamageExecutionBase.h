// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecutionBase.generated.h"

/**
 * 
 */
UCLASS()
class MUZANO_API UDamageExecutionBase : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UDamageExecutionBase(const FObjectInitializer& ObjectInitializer);
	virtual	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
