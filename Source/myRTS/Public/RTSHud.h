// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSCharacter.h"
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

	TArray<ARTSCharacter*> SelectedCharacters;

	TArray<AActor*> SelectedActors;

	/**
	* Returns the array of actors inside, with a class filter.
	* @param InActors					Actors that should be filtred
	* @return OutActors					The actors according to selection rule
	*/
	template <typename ClassFilter>
	bool GetSpecificActorsByFilter(const TArray<AActor*>InActors, TArray<ClassFilter*>& OutActors);
};

