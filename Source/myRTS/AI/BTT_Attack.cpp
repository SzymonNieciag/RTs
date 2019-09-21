// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "MainCharacter.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BrainComponent.h>
#include <GameFramework/Controller.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/BehaviorTreeTypes.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <GameFramework/Actor.h>
#include "Interface/CanBeDamaged.h"
#include <GameFramework/Pawn.h>
#include <AIController.h>
#include <DrawDebugHelpers.h>
#include "RTSGameplayStatic.h"

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AActor *OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	AMainCharacter *MainCharacter = Cast<AMainCharacter>(OwnerPawn);

	if (MainCharacter)
	{	
		UObject *Object= OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(Enemy.GetSelectedKeyID());
		if (Object!=nullptr)
		{
			AActor *Actor = Cast<AActor>(Object);
			if (ICanBeDamaged* TheInterface = Cast<ICanBeDamaged>(Actor))
			{
				float x = URTSGameplayStatic::ApplyDamage(Actor, 10, OwnerPawn);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("left health %f"), x)); // check it work?!
			}
			DrawDebugLine(GetWorld(), MainCharacter->GetActorLocation(), Actor->GetActorLocation(), FColor::Green, false, 1, 0, 1);
			return EBTNodeResult::Succeeded;
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
