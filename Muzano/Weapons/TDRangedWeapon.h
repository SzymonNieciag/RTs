// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDWeaponBasic.h"
#include "TDRangedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MUZANO_API ATDRangedWeapon : public ATDWeaponBasic
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ATDRangedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void UseWeapon() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
