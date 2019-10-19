// Copyright 2019 Dan Kestranek.


#include "GDPlayerController.h"
#include "AbilitySystemComponent.h"
#include "GDPlayerState.h"

// Server only
void AGDPlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AGDPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}
