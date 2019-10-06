// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "RTSCharacter.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BrainComponent.h>
#include <GameFramework/Controller.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/BehaviorTreeTypes.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Interface/CanBeDamaged.h"
#include <GameFramework/Pawn.h>
#include <AIController.h>
#include <DrawDebugHelpers.h>
#include "RTSGameplayStatic.h"

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AActor *OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	ARTSCharacter *MainCharacter = Cast<ARTSCharacter>(OwnerPawn);

	if (MainCharacter)
	{	
		UObject *Object= OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(Enemy.GetSelectedKeyID());
		if (Object!=nullptr)
		{
			ARTSCharacter *TargetMainCharacter = Cast<ARTSCharacter>(Object);
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

void UBTT_Attack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		Enemy.ResolveSelectedKey(*BBAsset);
	}
}
