// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacter.h"
#include "RTSPlayerController.generated.h"

UCLASS()
class ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ARTSPlayerController();

	class ARTSHud *HUDPtr;

	TArray<AUnitsSquad*> SelectedSquads;

protected:

	void SelectionPressed();
	void SelectionRelased();

	void MoveReleased();

	virtual void BeginPlay()override;
	//virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
};

