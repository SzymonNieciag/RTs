// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "MainCharacter.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTree.h>

AMainCharacterController::AMainCharacterController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BlackboardComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BehaviorComp"));
}

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

}

void AMainCharacterController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	AMainCharacter* Char = Cast<AMainCharacter>(InPawn);
	if (Char && Char->BehaviorTree->BlackboardAsset)
	{
		//Initialize the blackboard values
		BlackboardComp->InitializeBlackboard(*Char->BehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*Char->BehaviorTree);
	}
}
  

void AMainCharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	AMainCharacter *MainCharacter = Cast<AMainCharacter>(GetPawn());

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("koniec chodzenia")));

	if (MainCharacter)
	{
		BehaviorTreeComp->StartTree(*MainCharacter->BehaviorTree);
	}
}

void AMainCharacterController::SetGenericTeamId(const FGenericTeamId & TeamID)
{
	AITeamID = TeamID;
}


FGenericTeamId AMainCharacterController::GetGenericTeamId() const
{
	return AITeamID;
}
