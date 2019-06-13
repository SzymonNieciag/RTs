// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainCharacter.h"
#include "RTSHud.generated.h"

/**
 * 
 */
UCLASS()
class MYRTS_API ARTSHud : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD()override;

	FVector2D InitialPoint;

	FVector2D CurrentPoint;

	FVector2D GetMousePos2D();

	bool bStartingSelecting = false;

	TArray<AMainCharacter*> SelectedActors;
};
