// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Idle.generated.h"

/**
 * 
 */
UCLASS()
class MYRTS_API UBTS_Idle : public UBTService
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector CurrentEnemy;
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector CurrentState;

protected:

	UPROPERTY()
		AActor* Enemy;

public:

	/** called when auxiliary node becomes active
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	/** called when auxiliary node becomes inactive
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData)override;

	virtual void TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

	virtual void InitializeFromAsset(UBehaviorTree & Asset) override;
};
