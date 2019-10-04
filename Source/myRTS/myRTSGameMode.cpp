// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "myRTSGameMode.h"
#include "RTSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AmyRTSGameMode::AmyRTSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARTSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/CameraPawn_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}