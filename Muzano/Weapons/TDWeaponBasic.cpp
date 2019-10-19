// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWeaponBasic.h"
#include <Components/StaticMeshComponent.h>

// Sets default values
ATDWeaponBasic::ATDWeaponBasic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATDWeaponBasic::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATDWeaponBasic::UseWeapon()
{
}

// Called every frame
void ATDWeaponBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

