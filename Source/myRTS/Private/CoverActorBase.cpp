// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverActorBase.h"
#include "myRTSGameMode.h"
#include <Components/SceneComponent.h>

// Sets default values
ACoverActorBase::ACoverActorBase()
{

	SafeLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SafeLocation"));
	RootComponent = SafeLocation;
}

// Called when the game starts or when spawned
void ACoverActorBase::BeginPlay()
{
	Super::BeginPlay();
	AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
	RTSGameMode->AllCoverActors.Add(this);
}

void ACoverActorBase::Destroyed()
{
	Super::Destroyed();

	AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("The point behind the cover was destroyed")));

	if (RTSGameMode)
	{
		RTSGameMode->AllCoverActors.Remove(this);
	}
}
