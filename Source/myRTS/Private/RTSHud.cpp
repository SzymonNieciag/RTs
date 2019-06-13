// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSHud.h"
#include <GameFramework/HUD.h>
#include <GameFramework/PlayerController.h>
#include "MainCharacter.h"

void ARTSHud::DrawHUD()
{
	if (bStartingSelecting)
	{
		CurrentPoint = GetMousePos2D();
		DrawRect(FLinearColor(1,0,0,0.3f), InitialPoint.X, InitialPoint.Y, CurrentPoint.X - InitialPoint.X, CurrentPoint.Y - InitialPoint.Y);
		//GetActorsInSelectionRectangle<AMainCharacter>(InitialPoint, CurrentPoint, FoundActors, false, false);
		SelectedActors.Empty();
		GetActorsInSelectionRectangle<AMainCharacter>(InitialPoint, CurrentPoint, SelectedActors, false, false);
	}
}

FVector2D ARTSHud::GetMousePos2D()
{
	float PosX;
	float PosY;

	GetOwningPlayerController()->GetMousePosition(PosX,PosY);

	return FVector2D(PosX, PosY);
}
