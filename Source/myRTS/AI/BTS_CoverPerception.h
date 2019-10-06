// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "BTS_CoverPerception.generated.h"

/**
 * 
 */
class ACoverGoalPoint;
class ARTSCharacter;

UCLASS()
class MYRTS_API UBTS_CoverPerception : public UBTService
{
	GENERATED_BODY()
	
public:

	/*Enemy Actor*/
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Enemy;
	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector Goal;

	/** called when auxiliary node becomes active
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

	/** called when auxiliary node becomes inactive
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)override;

	virtual void OnSearchStart(FBehaviorTreeSearchData &SearchData)override;

	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	/* Search Closest CoversBaseActors and if Enemy is Valid additional scaning safe location*/
	void SearchCoverActors(UBehaviorTreeComponent & OwnerComp);

	void GetCoverPointsInRange(ARTSCharacter *OwnerPawn, TArray<ACoverGoalPoint*> &OutActors, float Range);

	virtual void InitializeFromAsset(UBehaviorTree &Asset) override;

};
