// Fill out your copyright notice in the Description page of Project Settings.


#include "TDRangedWeapon.h"

ATDRangedWeapon::ATDRangedWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDRangedWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ATDRangedWeapon::UseWeapon()
{

}

// Called every frame
void ATDRangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

