// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TSPickupActor.generated.h"

UCLASS()
class MUZANO_API ATSPickupActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATSPickupActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*the pickup to spawn*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<class AActor> PickableActorClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }
};
