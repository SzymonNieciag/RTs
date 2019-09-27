// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "MainCharacterController.generated.h"

/**
 * 
 */
class AMainCharacter;

UENUM(BlueprintType)
enum class ETeamType :uint8
{
	Red = 0 UMETA(DisplayName = "Red"),
	Blue = 1 UMETA(DisplayName = "Blue"),
	Teal = 2 UMETA(DisplayName = "Teal"),
	Purple = 3 UMETA(DisplayName = "Purple"),
	Yellow = 4 UMETA(DisplayName = "Yellow"),
	Orange = 5 UMETA(DisplayName = "Orange"),
	Green = 6 UMETA(DisplayName = "Green"),
	LightPink = 7 UMETA(DisplayName = "Light Pink"),
};

UCLASS()
class MYRTS_API AMainCharacterController : public AAIController
{
	GENERATED_BODY()
	
public: 

	AMainCharacterController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay()override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	//	UBehaviorTreeComponent *BehaviorTreeComp;

	virtual void OnPossess(APawn* InPawn)override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)override;

	virtual void MoveToLocationRTS(FVector Destination);

	virtual bool RunBehaviorTree(UBehaviorTree* BTAsset) override;

	UPROPERTY()
	AMainCharacter* MainCharacter;

private:

	FGenericTeamId AITeamID;

public:

	UFUNCTION(BlueprintCallable,Category = "AI")
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual FGenericTeamId GetGenericTeamId()const override;
};

//AAIControllerBase();
//virtual void OnPossess(APawn* InPawn)override;
//
//private:
//
//	UPROPERTY(EditAnywhere, Category = "AI")
//		FTeamProperties TeamProperties;
//
//public:
//	UFUNCTION(BlueprintCallable, Category = "AI")
//		virtual void SetTeamProperties(const FTeamProperties TeamProp);
//
//	/*UFUNCTION()
//	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID);
//	*/
//	/** Retrieve team identifier in form of FGenericTeamId */
//	UFUNCTION(BlueprintCallable, Category = "AI")
//		virtual FTeamProperties GetTeamProperties()const;
//
//	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor & Other) const override; do aiperception