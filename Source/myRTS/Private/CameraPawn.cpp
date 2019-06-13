// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "myRTSPlayerController.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-50, 0, 0));


	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom);

}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AmyRTSPlayerController>(GetController());
}

FVector ACameraPawn::GetCameraPawnDirection()
{
	float MousePosX;
	float MousePosY;

	float CameraDirectionX = 0;
	float CameraDirectionY = 0;

	PlayerController->GetMousePosition(MousePosX, MousePosY);

	if (MousePosX<=Margin)
	{
		CameraDirectionX=1;
	}
	if (MousePosX >= ScreenSizeX - Margin)
	{
		CameraDirectionX = -1;
	}
	if (MousePosY <= Margin)
	{
		CameraDirectionY = 1;
	}
	if (MousePosY >= ScreenSizeY - Margin)
	{
		CameraDirectionY = -1;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("attack %f %f"),MousePosX,MousePosY));
	return FVector(CameraDirectionX, CameraDirectionY, 0);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(GetCameraPawnDirection()*5);

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

