// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include "UnitsSquad.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTreeReference"));

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MainMesh->SetupAttachment(RootComponent);
	MainMesh->RelativeLocation = FVector(0,0,-90);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent *MovementPtr = Cast<UCharacterMovementComponent>(GetCharacterMovement());

	MovementPtr->MaxWalkSpeed = Speed;
}

void AMainCharacter::SetDamage_Implementation(float Damage, AActor * DamageCauser)
{
	Healt -= Damage;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("zycie = %f"), Healt));

	if (Healt<=0)
	{
		Destroy();
	}
}

void AMainCharacter::EnableDecalEffect_Implementation()
{

}

void AMainCharacter::DisableDecalEffect_Implementation()
{

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::Destroyed()
{
	Super::Destroyed();

	if (UnitsSquad != NULL)
	{
		UnitsSquad->RemoveFromSquad(this);
	}
}


