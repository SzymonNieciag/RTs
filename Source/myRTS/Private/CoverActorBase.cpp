// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverActorBase.h"
#include <Components/SphereComponent.h>
#include "myRTSGameMode.h"

// Sets default values
ACoverActorBase::ACoverActorBase()
{

	SafeLocation = CreateDefaultSubobject<USphereComponent>(TEXT("SafeLocation"));
	RootComponent = SafeLocation;
	CoverLocation = CreateDefaultSubobject<USphereComponent>(TEXT("CoverLocation"));
	CoverLocation->SetupAttachment(RootComponent);

	SafeLocation->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SafeLocation->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("cyka cyka")));

	if (RTSGameMode)
	{
		RTSGameMode->AllCoverActors.Remove(this);
	}
}

void ACoverActorBase::BeginDestroy()
{
	Super::BeginDestroy();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("fssdgsdg cyka")));

}

