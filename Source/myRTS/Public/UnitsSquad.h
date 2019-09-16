// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitsSquad.generated.h"

UCLASS()
class MYRTS_API AUnitsSquad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitsSquad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category = "Squad")
	TArray <AActor*> TeamMembers;

	UPROPERTY(EditAnywhere, Category = "Squad")
	int TeamSize;

public:

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void RemoveFromSquad(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "Squad")
	bool TryAddToSquad(AActor* actor);
};
