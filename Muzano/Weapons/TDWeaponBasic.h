// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDWeaponBasic.generated.h"

UCLASS()
class MUZANO_API ATDWeaponBasic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATDWeaponBasic();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent *StaticMeshComponent;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UseWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
