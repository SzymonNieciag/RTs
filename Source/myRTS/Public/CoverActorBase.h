// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoverActorBase.generated.h"


USTRUCT()
struct FCoverData
{
	GENERATED_BODY()
	/** Index into the material on the components data */
	UPROPERTY()
		AActor* CurrentActor = nullptr;
};

UCLASS()
class MYRTS_API ACoverActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoverActorBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RTS|CoverSystem")
		class USphereComponent* SafeLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RTS|CoverSystem")
		class USphereComponent* CoverLocation;

	FCoverData CoverData;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void BeginDestroy() override;

};
