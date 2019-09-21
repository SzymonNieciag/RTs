// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "myRTSGameMode.generated.h"

class ACoverActorBase;

UCLASS(minimalapi)
class AmyRTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AmyRTSGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
		TArray<AActor*> AllPawns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
		 TArray<ACoverActorBase*> AllCoverActors;
};



