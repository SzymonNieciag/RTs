// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "MainCharacter.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include "myRTSGameMode.h"

AMainCharacterController::AMainCharacterController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{/*
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BlackboardComp"));*/
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BehaviorComp"));
}

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

	//AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetPawn());
	//RunBehaviorTree(MainCharacter->BehaviorTree);
}

void AMainCharacterController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	if (GetWorld()) {
		AmyRTSGameMode* RTSGameMode = (AmyRTSGameMode*)GetWorld()->GetAuthGameMode();
		RTSGameMode->AllPawns.Add(this->GetPawn());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("polskapolksakokopkpkopkopk")));

	}

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(InPawn);
	if (MainCharacter->BehaviorTree->BlackboardAsset)
	{
		//Initialize the blackboard values
		Blackboard->InitializeBlackboard(*MainCharacter->BehaviorTree->BlackboardAsset);
	}
}

bool AMainCharacterController::RunBehaviorTree(UBehaviorTree * BTAsset)
{
	Super::RunBehaviorTree(BTAsset);

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetPawn());
	if (MainCharacter)
	{
		MainCharacter->BehaviorTree = BTAsset;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("sdgsgdgd ")));
		return true;
	}
	return false;
}

void AMainCharacterController::SetGenericTeamId(const FGenericTeamId & TeamID)
{
	AITeamID = TeamID;
}

FGenericTeamId AMainCharacterController::GetGenericTeamId() const
{
	return AITeamID;
}

void AMainCharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	AMainCharacter *MainCharacter = Cast<AMainCharacter>(GetPawn());

	if (MainCharacter)
	{
		RunBehaviorTree(MainCharacter->BehaviorTree);
	}
}

void AMainCharacterController::OnBeginMovement()
{

	GetBrainComponent()->StopLogic("stop");

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("start chodzenie")));

	//MainCharacterController->BehaviorTreeComp->StartTree(*x->BehaviorTree);
}

//AAIControllerBase::AAIControllerBase()
//{
//	TeamProperties.TeamName = "BOT";
//}
//
//void AAIControllerBase::OnPossess(APawn* InPawn)
//{
//	Super::OnPossess(InPawn);
//}
//
//void AAIControllerBase::SetTeamProperties(const FTeamProperties TeamProp)
//{
//	TeamProperties = TeamProp;
//
//}
//
//FTeamProperties AAIControllerBase::GetTeamProperties() const
//{
//	return TeamProperties;
//}
//
//ETeamAttitude::Type AAIControllerBase::GetTeamAttitudeTowards(const AActor & Other) const
//{
//	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {
//
//		if (const AAIControllerBase* BaseController = Cast<AAIControllerBase>(OtherPawn->GetController()))
//		{
//			FTeamProperties OtherTeamProperties = BaseController->GetTeamProperties();
//
//
//			if (OtherTeamProperties.AITeamID == TeamProperties.AITeamID)
//			{
//				return ETeamAttitude::Friendly;
//			}
//			for (int32 Team : TeamProperties.EnemyListID)
//			{
//				if (Team == OtherTeamProperties.AITeamID)
//				{
//					return ETeamAttitude::Hostile;
//				}
//			}
//			for (int32 Team : TeamProperties.FriendListID)
//			{
//				if (Team == OtherTeamProperties.AITeamID)
//				{
//					return ETeamAttitude::Friendly;
//				}
//			}
//			return ETeamAttitude::Neutral;
//		}
//		return ETeamAttitude::Neutral;
//	}
//	return ETeamAttitude::Neutral;
//}  ovveride Aiperceptions
