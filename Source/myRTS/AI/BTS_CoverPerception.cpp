// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CoverPerception.h"
#include "myRTSGameMode.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BTNode.h>
#include <Engine/World.h>
#include <BrainComponent.h>
#include <GameFramework/Controller.h>
#include <AIController.h>
#include "CoverActorBase.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>
#include "MainCharacter.h"

void UBTS_CoverPerception::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

}

void UBTS_CoverPerception::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTS_CoverPerception::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

}

void UBTS_CoverPerception::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMainCharacter *MainCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	UWorld *World = GetWorld();
	if (World)
	{
		TArray<ACoverActorBase*> CoverActors;
		GetCoverPointsInRange(MainCharacter, CoverActors, 500);
		CoverActors.Sort();

		for (ACoverActorBase* CoverActor : CoverActors)
		{
			if (!MainCharacter->CoverActor && !CoverActor->CoverData.CurrentActor)
			{
				MainCharacter->CoverActor = CoverActor;
				CoverActor->CoverData.CurrentActor = MainCharacter;
				DrawDebugLine(GetWorld(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), CoverActor->GetActorLocation(), FColor::Green, false, 1, 0, 1);
			}
		}
	}
}

void UBTS_CoverPerception::GetCoverPointsInRange(AMainCharacter *OwnerPawn, TArray<ACoverActorBase*>& OutActors, float Range)
{
	AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
	for (auto CoverActor : RTSGameMode->AllCoverActors)
	{
		float Range = 500; // add after as public value
		float SquaredRange = Range * Range;
		if (OwnerPawn->GetSquaredDistanceTo(CoverActor) < SquaredRange)
		{
			OutActors.Add(CoverActor);
		}
	}
}

void UBTS_CoverPerception::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		//CurrentCoverActor.ResolveSelectedKey(*BBAsset);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("inizjalizacja w perception")));
	}
	bNotifyBecomeRelevant = true;
}
