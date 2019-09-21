// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsSelectable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsSelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYRTS_API IIsSelectable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Decal")
		void EnableDecalEffect();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Decal")
		void DisableDecalEffect();
};
