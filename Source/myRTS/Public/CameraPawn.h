// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/DefaultPawn.h>
#include "CameraPawn.generated.h"

UCLASS()
class MYRTS_API ACameraPawn : public ADefaultPawn
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	APlayerController* PlayerController;

	void Zoom(float Val);
	void MoveForward(float Val) override;
	void MoveRight(float Val) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "25.0"))
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "100.0"))
	float ScreenEdgeZonePercent;

	bool MoveForwardKeyboard = false;
	bool MoveRightKeyboard = false;
};
