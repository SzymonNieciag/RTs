// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitsSquad.h"

// Sets default values
AUnitsSquad::AUnitsSquad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitsSquad::BeginPlay()
{
	Super::BeginPlay();

	TeamSize = 1;
}

// Called every frame
void AUnitsSquad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnitsSquad::RemoveFromSquad(AActor* actor)
{
	TeamMembers.Remove(actor);

	if (TeamMembers.Num()==0)
	{
		Destroy(this);
	}
}

bool AUnitsSquad::TryAddToSquad(AActor* actor)
{
	if (TeamMembers.Num() < TeamSize)
	{
		if (actor->IsA(AActor::StaticClass()))
		{
			TeamMembers.Add(actor);
			return true;
		}
		return false;
	}
	else
		return false;
}
