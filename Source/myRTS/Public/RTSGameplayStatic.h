// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTSGameplayStatic.generated.h"

/**
 * 
 */
UCLASS()
class MYRTS_API URTSGameplayStatic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "RTS|Damage")
	static float ApplyDamage(AActor* DamagedActor, float BaseDamage, AActor* DamageCauser);
};

