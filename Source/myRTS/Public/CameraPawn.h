// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <GameFramework/PlayerController.h>
#include "CameraPawn.generated.h"

UCLASS()
class MYRTS_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:

	ACameraPawn();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) override;

	virtual void BeginPlay()override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

public:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USceneComponent* RootScene;
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	    APlayerController* PlayerController;

	UFUNCTION()
		FVector GetCameraPawnDirection();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float Margin = 15;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	int32 ScreenSizeX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	int32 ScreenSizeY;

};
