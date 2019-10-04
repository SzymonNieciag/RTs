// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "RTSPlayerController.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-50, 0, 0));


	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom);

	MoveSpeed = 5;
	ScreenEdgeZonePercent = 15;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ARTSPlayerController>(GetController());
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller && !MoveRightKeyboard && !MoveForwardKeyboard)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());

		if (playerController != nullptr)
		{
			FVector2D mouseLocation;
			if (playerController->GetMousePosition(mouseLocation.X, mouseLocation.Y))
			{
				int32 screenWidth = 0;
				int32 screenHeight = 0;

				playerController->GetViewportSize(screenWidth, screenHeight);

				const float xZone = screenWidth * ScreenEdgeZonePercent / 100;
				const float yZone = screenHeight * ScreenEdgeZonePercent / 100;

				FVector Direction;

				if (mouseLocation.X < xZone)							Direction = FVector((FVector::LeftVector));
				else if (mouseLocation.X > screenWidth - xZone)		    Direction = FVector((FVector::RightVector));
				if (mouseLocation.Y < yZone)							Direction = FVector(Direction + FVector::ForwardVector);
				else if (mouseLocation.Y > screenHeight - yZone)		Direction = FVector(Direction + FVector::BackwardVector);

				this->AddActorLocalOffset(Direction * MoveSpeed);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Some variable values: x: %f, y: %f"), mouseLocation.X, mouseLocation.Y));
			}
		}
	}
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("Mouse Wheel", this, &ACameraPawn::Zoom);
}

//void ACameraPawnRTS::Tick(float DeltaTime)
//{
//	
//}
void ACameraPawn::Zoom(float magnitude)
{
	if (magnitude != 0 && Controller)
	{

	}
}

void ACameraPawn::MoveForward(float Val)
{
	if (Val != 0 && Controller)
	{
		MoveForwardKeyboard = true;
		this->AddActorLocalOffset(FVector::ForwardVector * Val * MoveSpeed);
	}
	else
	{
		MoveForwardKeyboard = false;
	}
}

void ACameraPawn::MoveRight(float Val)
{
	if (Val != 0 && Controller)
	{
		MoveRightKeyboard = true;
		this->AddActorLocalOffset(FVector::RightVector * Val * MoveSpeed);
	}
	else
	{
		MoveRightKeyboard = false;
	}
}
