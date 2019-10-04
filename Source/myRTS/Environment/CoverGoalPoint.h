// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverGoalPoint.generated.h"


USTRUCT(BlueprintType)
struct FCoverData
{
	GENERATED_BODY()
	/** Index into the material on the components data */
	UPROPERTY(BlueprintReadOnly)
		APawn* DestinateTargetPawn = nullptr;
	/* Reduce Chance to Hit 0 = block full Damage 1 = Nothing to Block 0.3 Block 70% from damage*/
	UPROPERTY(BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float ReduceChanceToHit = 0.5f;

	FCoverData() : DestinateTargetPawn(nullptr), ReduceChanceToHit(0.5f) {};
};

UCLASS()
class MYRTS_API ACoverGoalPoint : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACoverGoalPoint();

	FCoverData CoverData;

	UFUNCTION(BlueprintPure)
	APawn* GetDestinateTargetActor() { return CoverData.DestinateTargetPawn; }

	UFUNCTION(BlueprintPure)
	float GetReduceChanceToHit() { return CoverData.ReduceChanceToHit; }

	void SetDestinateTargetActor(APawn* Pawn);

	UFUNCTION()
	void RemoveDestinateActor(APawn* Pawn);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:

	/* Distance to current destinate actor */
	float DistanceSqrtmp;
};
