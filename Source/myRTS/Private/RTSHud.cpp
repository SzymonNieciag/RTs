// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSHud.h"
#include <GameFramework/HUD.h>
#include <GameFramework/PlayerController.h>
#include "RTSCharacter.h"
#include "RTSGameplayStatic.h"

void ARTSHud::DrawHUD()
{
	if (bStartingSelecting)
	{
		CurrentPoint = GetMousePos2D();
		DrawRect(FLinearColor(1,0,0,0.3f), InitialPoint.X, InitialPoint.Y, CurrentPoint.X - InitialPoint.X, CurrentPoint.Y - InitialPoint.Y);

		SelectedActors.Empty();

		GetActorsInSelectionRectangle(InitialPoint, CurrentPoint, SelectedActors, false, false);

		GetSpecificActorsByFilter(SelectedActors, SelectedCharacters);

		if (SelectedActors.Num() != 0)
		{
			for (auto* PreviewActor : SelectedActors)
			{
				if (IIsSelectable* TheInterface = Cast<IIsSelectable>(PreviewActor))
				{
					IIsSelectable::Execute_ReTriggerPreviewDecal(PreviewActor);
				}
			}
		}
	}
	else
	{
		CurrentPoint = GetMousePos2D();
		InitialPoint = FVector2D(CurrentPoint.X + 1, CurrentPoint.Y + 1);

		SelectedActors.Empty();

		GetActorsInSelectionRectangle(InitialPoint, CurrentPoint, SelectedActors, false, false);

		if (SelectedActors.Num() != 0)
		{
			if (IIsSelectable* TheInterface = Cast<IIsSelectable>(SelectedActors[0]))
			{
				IIsSelectable::Execute_ReTriggerPreviewDecal(SelectedActors[0]);
			}
		}
	}
}

FVector2D ARTSHud::GetMousePos2D()
{
	float PosX;
	float PosY;

	GetOwningPlayerController()->GetMousePosition(PosX,PosY);

	return FVector2D(PosX, PosY);
}

template <typename ClassFilter>
bool ARTSHud::GetSpecificActorsByFilter(const TArray<AActor*>InActors, TArray<ClassFilter*>& OutActors)
{
	OutActors.Empty();

	//Is Actor subclass?
	if (!ClassFilter::StaticClass()->IsChildOf(AActor::StaticClass()))
	{
		return false;
	}

	//Construct casted template type array
	for (AActor* EachActor : InActors)
	{
		if (auto *test = Cast<ClassFilter>(EachActor))
		{
			OutActors.Add(CastChecked<ClassFilter>(test));
		}
	}
	return true;
}
