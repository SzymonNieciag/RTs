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
	SearchCoverActors(SearchData.OwnerComp);
}

void UBTS_CoverPerception::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	SearchCoverActors(OwnerComp);
}

void UBTS_CoverPerception::GetCoverPointsInRange(AMainCharacter *OwnerPawn, TArray<ACoverActorBase*>& OutActors, float Range)
{
	AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
	for (auto CoverActor : RTSGameMode->AllCoverActors)
	{
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

void UBTS_CoverPerception::SearchCoverActors(UBehaviorTreeComponent & OwnerComp)
{
	AMainCharacter *MainCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MainCharacter->CoverActor)
	{
		TArray<ACoverActorBase*> CoverActors;
		GetCoverPointsInRange(MainCharacter, CoverActors, 500);
		CoverActors.Sort();
		for (ACoverActorBase* CoverActor : CoverActors)
		{
			if (!CoverActor->CoverData.CurrentActor)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("GOGO To Cover")));
				MainCharacter->CoverActor = CoverActor;
				CoverActor->CoverData.CurrentActor = MainCharacter;

				DrawDebugSphere(GetWorld(), CoverActor->GetActorLocation(), 25, 10, FColor::Blue, 3, 25);
				DrawDebugLine(GetWorld(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), CoverActor->GetActorLocation(), FColor::Blue, false, 1, 0, 5.0f);
				return;
			}
		}
	}
}