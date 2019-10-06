// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "RTSCharacter.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include "myRTSGameMode.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <DrawDebugHelpers.h>
#include <BrainComponent.h>
#include "Environment/CoverGoalPoint.h"

AMainCharacterController::AMainCharacterController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BlackboardComp"));
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
	}
}

bool AMainCharacterController::RunBehaviorTree(UBehaviorTree * BTAsset)
{
	Super::RunBehaviorTree(BTAsset);

	ARTSCharacter* MainCharacter = Cast<ARTSCharacter>(GetPawn());
	if (MainCharacter)
	{
		MainCharacter->BehaviorTree = BTAsset;
		return true;
	}
	return false;
}

void AMainCharacterController::SetGenericTeamId(const FGenericTeamId & NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);
	//AITeamID = NewTeamID;
}

FGenericTeamId AMainCharacterController::GetGenericTeamId() const
{
	return Super::GetGenericTeamId();
}

void AMainCharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	this->GetBlackboardComponent()->SetValueAsBool("PriorityOrder", false);
	ARTSCharacter *MainCharacter = Cast<ARTSCharacter>(GetPawn());

	if (MainCharacter)
	{
		if (UObject* Object = GetBrainComponent()->GetBlackboardComponent()->GetValueAsObject("Goal"))
		{
			ACoverGoalPoint *CoverActorPoint = Cast<ACoverGoalPoint>(Object);
			if (CoverActorPoint)
			{
				if (MainCharacter->CheckCoverPoint(CoverActorPoint))
				{
					MainCharacter->CoverPoint = CoverActorPoint;
					DrawDebugSphere(GetWorld(), MainCharacter->CoverPoint->GetActorLocation(), 40, 3, FColor::Red, false, 3.0f, 25.0f);
				}
				else
				{
					MainCharacter->CoverPoint = nullptr;
				}
			}
		}
	}
	OnStopMovement.Broadcast(MainCharacter);
}

void AMainCharacterController::MoveToLocationRTS(FVector Destination)
{
	MoveToLocation(Destination, 30, true, true, false);

	this->GetBlackboardComponent()->SetValueAsBool("PriorityOrder", true);
	ARTSCharacter *MainCharacter = Cast<ARTSCharacter>(GetPawn());

	if (MainCharacter && MainCharacter->CoverPoint)
	{
		MainCharacter->LeaveTheCover();
	}
}

FAIRequestID AMainCharacterController::RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path)
{
	ARTSCharacter *MainCharacter = Cast<ARTSCharacter>(GetPawn());
	if (MainCharacter && MainCharacter->CoverPoint)
	{
		MainCharacter->LeaveTheCover();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("wyjscie z covera")));

	}
	return Super::RequestMove(MoveRequest, Path);
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
