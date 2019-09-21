// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "myRTSPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Engine/World.h"
#include "RTSHud.h"
#include <AI/NavigationSystemBase.h>
#include <DrawDebugHelpers.h>
#include "MainCharacter.h"
#include "MainCharacterController.h"
#include <NavigationSystem.h>

enum EDirection
{
	Right, Up, Left, Down
};

AmyRTSPlayerController::AmyRTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
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

void AmyRTSPlayerController::SelectionPressed()
{
	if (HUDPtr != nullptr)
	{
		for (AActor* Actor : HUDPtr->SelectedActors)
		{
			if (IIsSelectable* TheInterface = Cast<IIsSelectable>(Actor))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("stop ")));
				IIsSelectable::Execute_DisableDecalEffect(Actor);
			}
		}
		HUDPtr->bStartingSelecting = true;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("start ")));
		HUDPtr->InitialPoint = HUDPtr->GetMousePos2D();
	}
}

void AmyRTSPlayerController::SelectionRelased()
{
	if (HUDPtr != nullptr)
	{
		HUDPtr->bStartingSelecting = false;
		for (AActor* Actor : HUDPtr->SelectedActors)
		{
			if (IIsSelectable* TheInterface = Cast<IIsSelectable>(Actor))
			{
				IIsSelectable::Execute_EnableDecalEffect(Actor);
			}
		}
	}
}

void AmyRTSPlayerController::MoveReleased()
{
	if (HUDPtr->SelectedActors.Num() != 0)
	{
		if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
		{
			if (ANavigationData* NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate))
			{
				int32 MaxIndex = HUDPtr->SelectedActors.Num();
				FHitResult HitResult;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

				EDirection Direction = EDirection::Right;
				int Cout = 1;
				int Max = 1;

				float LocationX = HitResult.Location.X;
				float LocationY = HitResult.Location.Y;

				int32 MaxNumberofProbes = MaxIndex;
				int32 PawnTurn = 0;

				AMainCharacter *MainCharacter = Cast<AMainCharacter>(HUDPtr->SelectedActors[PawnTurn]);
				float UnitSize = MainCharacter->NavLocationSize;

				///////////////////
				FVector CheckLocation = FVector(LocationX, LocationY, HitResult.Location.Z);
				FVector CorrectLocation;

				// Check ProjectPoint if he is moveable. Don't setup FVector(0,0,0)!.
				if (UNavigationSystemV1::K2_ProjectPointToNavigation(GetWorld(), CheckLocation, CorrectLocation, NavData, nullptr, FVector(50, 50, 50)))
				{
					UAIBlueprintHelperLibrary::SimpleMoveToLocation(HUDPtr->SelectedActors[PawnTurn]->GetController(), CorrectLocation);

					AMainCharacterController *MainCharacterController = Cast<AMainCharacterController>(HUDPtr->SelectedActors[0]->GetController());
					MainCharacterController->OnBeginMovement();

					DrawDebugSphere(GetWorld(), CheckLocation, 25, 10, FColor::Blue, 3, 5);

					PawnTurn++;
				}
				else
					return;

				for (int i = 1; i < MaxNumberofProbes; i++)
				{

					if (Direction == Right)
					{
						Cout--;
						if (Cout == 0)
						{
							Cout = Max;
							Direction = Up;
						}
						LocationX -= UnitSize;
					}
					else if (Direction == Up)
					{
						Cout--;
						if (Cout == 0)
						{
							Max++;
							Cout = Max;
							Direction = Left;
						}
						LocationY += UnitSize;

					}
					else if (Direction == Left)
					{
						Cout--;
						if (Cout == 0)
						{
							Cout = Max;
							Direction = Down;
						}
						LocationX += UnitSize;
					}
					else
					{
						Cout--;
						if (Cout == 0)
						{
							Max++;
							Cout = Max;
							Direction = Right;
						}
						LocationY -= UnitSize;
					}


					CheckLocation = FVector(LocationX, LocationY, HitResult.Location.Z);

					if (UNavigationSystemV1::K2_ProjectPointToNavigation(GetWorld(), CheckLocation, CorrectLocation, NavData, nullptr, FVector(50, 50, 50)))
					{
						UAIBlueprintHelperLibrary::SimpleMoveToLocation(HUDPtr->SelectedActors[PawnTurn]->GetController(), CorrectLocation);

						AMainCharacterController *MainCharacterController = Cast<AMainCharacterController>(HUDPtr->SelectedActors[PawnTurn]->GetController());
						MainCharacterController->OnBeginMovement();

						DrawDebugSphere(GetWorld(), CorrectLocation, 25, 10, FColor::Green, 3, 5);

						PawnTurn++;
					}
					else
					{
						DrawDebugSphere(GetWorld(), CorrectLocation, 25, 10, FColor::Blue, 3, 5);
						MaxNumberofProbes++;
					}
				}
			}
		}
	}
}

void AmyRTSPlayerController::BeginPlay()
{
	HUDPtr = Cast<ARTSHud>(GetHUD());
}

