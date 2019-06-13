// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CanBeDamaged.h"
#include <BehaviorTree/BehaviorTree.h>
#include "MainCharacter.generated.h"

UCLASS()
class MYRTS_API AMainCharacter : public ACharacter, public ICanBeDamaged
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Mesh")
	UStaticMeshComponent *MainMesh;

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Stat")
		float Healt = 100;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float Attack = 10;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float Armor = 10;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float RangeAttack = 200;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float Speed = 400;
	UPROPERTY(EditAnywhere, Category = "Stat")
		bool isAlive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MyCategory")
		void SetDamage(float Damage, AActor* DamageCauser);
	virtual void SetDamage_Implementation(float Damage, AActor* DamageCauser) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
