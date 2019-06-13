// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class MYRTS_API AMainCharacterController : public AAIController
{
	GENERATED_BODY()
	
public: 

	AMainCharacterController();

	virtual void BeginPlay()override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
		UBlackboardComponent *BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
		UBehaviorTreeComponent *BehaviorTreeComp;

	virtual void OnPossess(APawn* InPawn)override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)override;

private:

		FGenericTeamId AITeamID;

public:

	UFUNCTION(BlueprintCallable,Category = "AI")
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual FGenericTeamId GetGenericTeamId()const override;
};
