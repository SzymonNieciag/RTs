// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Perception.h"
#include <GameFramework/Pawn.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Engine/World.h>
#include "MainCharacterController.h"
#include <Engine/Engine.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BrainComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "MainCharacter.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Enum.h>
#include <BehaviorTree/BehaviorTreeTypes.h>

void UBTS_Perception::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CurrentState.GetSelectedKeyID(), (uint8)EStateType::Idle);
}

void UBTS_Perception::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTS_Perception::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
}

void UBTS_Perception::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	Enemy = GetClosestEnemy(OwnerComp);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("tick perception")));

	if (Enemy!=nullptr)
	{
		AMainCharacter *MainCharacter = Cast<AMainCharacter>(OwnerComp.GetAIOwner()->GetPawn());

		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(CurrentEnemy.GetSelectedKeyID(), Enemy);
		if (MainCharacter)
		{
			float RangeAttack = MainCharacter->RangeAttack;

			if (MainCharacter->GetDistanceTo(Enemy)>= RangeAttack)
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CurrentState.GetSelectedKeyID(), (uint8)EStateType::Walk);
				
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("attack %d"), (uint8)EStateType::Attack));
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CurrentState.GetSelectedKeyID(), (uint8)EStateType::Attack);
			}
		}
	}
	else
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CurrentState.GetSelectedKeyID(), (uint8)EStateType::Walk);
}
void UBTS_Perception::GetAllEnemiesInRange(UBehaviorTreeComponent &OwnerComp, TArray<AActor*>& OutActors,float ViewRange)
{
	AActor *OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

		if (OwnerPawn)
		{
			UWorld *World = GetWorld();
			if (World)
			{
				TArray<AActor*> FoundPawns;
				UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), FoundPawns);

				FGenericTeamId OwnerTeam = OwnerComp.GetAIOwner()->GetGenericTeamId();/*
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("sssssssssssss %d"), OwnerTeam.GetId()));*/
				for (AActor* Actor : FoundPawns)
				{
					APawn *Pawn = Cast<APawn>(Actor);

					AMainCharacterController *MainCharacterController = Cast<AMainCharacterController>(Pawn->GetController());

					if (MainCharacterController)
					{
						if (OwnerTeam != MainCharacterController->GetGenericTeamId())
						{
							float Range = ViewRange * ViewRange;
							if (OwnerPawn->GetSquaredDistanceTo(Actor)<Range)
							{
								OutActors.Add(Pawn);
							}
						}
					}
				}
			}
		}
}

AActor* UBTS_Perception::GetClosestEnemy(UBehaviorTreeComponent & OwnerComp)
{
	AActor *OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (OwnerPawn)
	{
		TArray <AActor*> Enemies;
		GetAllEnemiesInRange(OwnerComp, Enemies,ViewRange);

		AActor *LocalEnemy = nullptr;
		float AveDistance = 1000000000;

		if (Enemies.Num()!=0)
		{
			for (AActor *Actor : Enemies)
			{
				float LocalDistance = Actor->GetSquaredDistanceTo(OwnerPawn);

				if (AveDistance > LocalDistance)
				{
					LocalEnemy = Actor;
					AveDistance = LocalDistance;
				}
			}
		}
		return LocalEnemy;
	}
	return nullptr;
}


void UBTS_Perception::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		CurrentEnemy.ResolveSelectedKey(*BBAsset);
		CurrentState.ResolveSelectedKey(*BBAsset);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("inizjalizacja w perception")));
	}
	bNotifyBecomeRelevant = true;
}