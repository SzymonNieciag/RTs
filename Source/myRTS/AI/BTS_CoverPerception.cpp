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

void UBTS_CoverPerception::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		Enemy.ResolveSelectedKey(*BBAsset);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("inizjalizacja w perception")));
	}
	bNotifyBecomeRelevant = true;
}

void UBTS_CoverPerception::SearchCoverActors(UBehaviorTreeComponent& OwnerComp)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!MainCharacter->IsCovered())
	{
		UObject* Object = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(Enemy.GetSelectedKeyID());
		AMainCharacter* Enemy = nullptr;
		if (Object)
		{
			Enemy = Cast<AMainCharacter>(Object);
		}
		if (Enemy)
		{
			TArray<ACoverActorBase*> CoverActors;

			
			GetCoverPointsInRange(MainCharacter, CoverActors, 1000.0f);

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("inizjalizacja w perception %d"),CoverActors.Num()));

			/* Lambda sort Array */
			CoverActors.Sort([](const ACoverActorBase& ip1, const ACoverActorBase& ip2) {
				return ip1.DistanceSqrtmp < ip2.DistanceSqrtmp;
			});/*
			int xx = 10;
			for (ACoverActorBase *CoverActor : CoverActors)
			{
				DrawDebugSphere(GetWorld(), CoverActor->GetActorLocation(), xx, 10, FColor::Blue, false, 3.0f, 25.0f);
				xx += 20;
			}*/

			/* Check if Found point isn't already set*/
			for (ACoverActorBase *CoverActor : CoverActors)
			{
				if (CoverActor->GetDestinateTargetActor())
				{
					if (CoverActor->GetDestinateTargetActor() == MainCharacter)
					{
						return;
					}
				}
				/* set new Cover Point */
				else if(!CoverActor->GetDestinateTargetActor())
				{
					MainCharacter->CoverActor = nullptr;
					CoverActor->SetDestinateTargetActor(nullptr);
					MainCharacter->CoverActor = CoverActor;
					CoverActor->SetDestinateTargetActor(MainCharacter);

					DrawDebugSphere(GetWorld(), CoverActor->GetActorLocation(), 25, 10, FColor::Blue, false, 3.0f, 25.0f);
					DrawDebugLine(GetWorld(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), CoverActor->GetActorLocation(), FColor::Blue, false, 1, 0, 5.0f);
					return;
				}
			}
		}
	}
}

void UBTS_CoverPerception::GetCoverPointsInRange(AMainCharacter *OwnerPawn, TArray<ACoverActorBase*> &OutActors, float Range)
{
	auto* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
	for (ACoverActorBase *CoverActor : RTSGameMode->AllCoverActors)
	{
		float SquaredRange = Range * Range;
		float Distancetmp = OwnerPawn->GetSquaredDistanceTo(CoverActor);
		if (Distancetmp < SquaredRange)
		{
			CoverActor->DistanceSqrtmp = Distancetmp;
			OutActors.Add(CoverActor);
		}
	}
}

//for (const TPair<int32, AActor*>& pair : exampleIntegerToActorMap)
//{
//	pair.Key;
//	pair.Value;
//}