// Fill out your copyright notice in the Description page of Project Settings.


#include "TSPickupActor.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SphereComponent.h>

// Sets default values
ATSPickupActor::ATSPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));

	RootComponent = SphereComponent;
	PickupMesh->AttachTo(RootComponent);

	PickupMesh->SetRelativeLocation(FVector(0,0,0));

	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionProfileName("OverlapOnlyPawn");

}

// Called when the game starts or when spawned
void ATSPickupActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATSPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATSPickupActor::WasCollected_Implementation()
{

}

