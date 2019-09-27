// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CoverPerception.generated.h"

/**
 * 
 */
class ACoverActorBase;
class AMainCharacter;

UCLASS()
class MYRTS_API UBTS_CoverPerception : public UBTService
{
	GENERATED_BODY()
	
	////Enemy Actor
	//UPROPERTY(EditAnywhere, Category = Blackboard)
	//	struct FBlackboardKeySelector CurrentCoverActor;

protected:

public:
	/* Max Distance Filter CoverActors*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI")
	float Range = 500; 
	/** called when auxiliary node becomes active
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

	/** called when auxiliary node becomes inactive
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)override;

	virtual void OnSearchStart(FBehaviorTreeSearchData &SearchData)override;

	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	void SearchCoverActors(UBehaviorTreeComponent & OwnerComp);

	void GetCoverPointsInRange(AMainCharacter *OwnerPawn, TArray<ACoverActorBase*> &OutActors, float Range);

	virtual void InitializeFromAsset(UBehaviorTree &Asset) override;
};
