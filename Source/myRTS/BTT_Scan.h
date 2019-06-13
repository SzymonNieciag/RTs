// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Scan.generated.h"

UCLASS()
class MYRTS_API UBTT_Scan : public UBTTaskNode
{
	GENERATED_BODY()

		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
//
//	/** initialize any asset related data */
//	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
//
//	/** get name of selected blackboard key */
//	FName GetSelectedBlackboardKey() const;
//
//protected:
//
//	/** blackboard key selector */
//	UPROPERTY(EditAnywhere, Category = Blackboard)
//		struct FBlackboardKeySelector Waypoint;
};
//
//FORCEINLINE FName UBTT_SelectNextPoint::GetSelectedBlackboardKey() const
//{
//	return Waypoint.SelectedKeyName;
//}
