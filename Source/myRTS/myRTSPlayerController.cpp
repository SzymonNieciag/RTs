// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "myRTSPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "myRTSCharacter.h"
#include "Engine/World.h"
#include "RTSHud.h"
#include <AI/NavigationSystemBase.h>
#include <DrawDebugHelpers.h>
#include "MainCharacter.h"
#include "MainCharacterController.h"

AmyRTSPlayerController::AmyRTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AmyRTSPlayerController::SelectionPressed()
{
	if (HUDPtr != nullptr)
	{
		HUDPtr->InitialPoint = HUDPtr->GetMousePos2D();
		HUDPtr->bStartingSelecting = true;
	}
}

void AmyRTSPlayerController::SelectionRelased()
{
	if (HUDPtr != nullptr)
	{
		HUDPtr->bStartingSelecting = false;
	}
}

void AmyRTSPlayerController::MoveReleased()
{
	if (HUDPtr->SelectedActors.Num()!=0)
	{
		int32 MaxIndex = HUDPtr->SelectedActors.Num();
		for (int32 i = 0; i<MaxIndex; i++)
		{

			FHitResult HitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
			FVector MoveLocation = HitResult.Location + FVector(i * 50, (i % 2) * 100, 0);

			UAIBlueprintHelperLibrary::SimpleMoveToLocation(HUDPtr->SelectedActors[i]->GetController(), MoveLocation);
			DrawDebugSphere(GetWorld(), MoveLocation, 25, 10, FColor::Red, 3);

		}
		for (AMainCharacter *x : HUDPtr->SelectedActors)
		{
			AMainCharacterController *MainCharacterController = Cast<AMainCharacterController>(x->GetController());
			MainCharacterController->BehaviorTreeComp->StopLogic("afas");

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("start chodzenie")));

			//MainCharacterController->BehaviorTreeComp->StartTree(*x->BehaviorTree);
		}
	}
}

void AmyRTSPlayerController::BeginPlay()
{
	HUDPtr = Cast<ARTSHud>(GetHUD());
}

void AmyRTSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AmyRTSPlayerController::SelectionPressed);
	InputComponent->BindAction("LeftMouseClick", IE_Released, this, &AmyRTSPlayerController::SelectionRelased);

	InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AmyRTSPlayerController::MoveReleased);
	//InputComponent->BindAction("RightMouseClick", IE_Released, this, &AmyRTSPlayerController::OnSetDestinationReleased);
}
