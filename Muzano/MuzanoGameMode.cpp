// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MuzanoGameMode.h"
#include "UObject/ConstructorHelpers.h"

AMuzanoGameMode::AMuzanoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_TDSPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
