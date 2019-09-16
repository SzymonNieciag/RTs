// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Squad.generated.h"

/**
 * 
 */
UCLASS()
class MYRTS_API USquad : public UObject
{
	GENERATED_BODY()

public:

	USquad();
	TArray <AActor*> TeamMembers;
	int TeamSize;

	float TeamExperience;
	int TeamLevel;
};
