// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverGoalPoint.h"
#include "myRTSGameMode.h"
#include <Components/SceneComponent.h>
#include "MainCharacterController.h"
#include "RTSCharacter.h"

// Sets default values
ACoverGoalPoint::ACoverGoalPoint()
{

}

void ACoverGoalPoint::SetDestinateTargetActor(APawn* Pawn)
{
	if (Pawn)
	{
		AMainCharacterController* MainCharacterController = Cast<AMainCharacterController>(Pawn->GetController());
		//MainCharacterController->OnStopMovement.AddDynamic(this, &ACoverGoalPoint::RemoveDestinateActor);
		CoverData.DestinateTargetPawn = Pawn;
	}
	else
	{
		CoverData.DestinateTargetPawn = nullptr;
	}
}

void ACoverGoalPoint::RemoveDestinateActor(APawn* Pawn)
{
	AMainCharacterController* MainCharacterController = Cast<AMainCharacterController>(Pawn->GetController());
	ARTSCharacter* MainCharacter = Cast<ARTSCharacter>(Pawn);
	if (!MainCharacter->IsCovered())
	{
		MainCharacterController->OnStopMovement.RemoveDynamic(this, &ACoverGoalPoint::RemoveDestinateActor);
		CoverData.DestinateTargetPawn = nullptr;
	}
	else
	{
		
	}
}

// Called when the game starts or when spawned
void ACoverGoalPoint::BeginPlay()
{
	Super::BeginPlay();
	AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
	RTSGameMode->AllCoverActors.Add(this);
}

void ACoverGoalPoint::Destroyed()
{
	Super::Destroyed();

	AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("The point behind the cover was destroyed")));

	if (RTSGameMode)
	{
		RTSGameMode->AllCoverActors.Remove(this);
	}
}
