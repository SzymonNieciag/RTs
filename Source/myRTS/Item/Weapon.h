// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FAmmoAccuracy
{
	GENERATED_BODY()

	//Always make USTRUCT variables into UPROPERTY()
	//any non-UPROPERTY() struct vars are not replicated
	// So to simplify your life for later debugging, always use UPROPERTY()
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float Distance;
	/* Accuracy from 0 - 100 */
	    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float Accuracy;

	//Always initialize your USTRUCT variables!
	//exception is if you know the variable type has its own default constructor
	FAmmoAccuracy() : Distance(100), Accuracy(50.0f) {};
	FAmmoAccuracy(int DistanceValue, float AccuracyValue) : Distance(DistanceValue), Accuracy(AccuracyValue) {};
}; 
USTRUCT(BlueprintType)
struct FWeaponAccuracy
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
		FAmmoAccuracy FirstThreshold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoAccuracy SecoundThreshold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FAmmoAccuracy ThirdThreshold;

	FWeaponAccuracy() : FirstThreshold(300.0f, 70.0f), SecoundThreshold(500.0f, 60.0f), ThirdThreshold(700.0f, 50.0f) {};
};

UCLASS()
class MYRTS_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon() :FireRate(20.0f), ShotRange(1000) { };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Bullets per Minute*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float FireRate;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float ShotRange;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Damage;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Ammo;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float ReloadingTime;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FWeaponAccuracy WeaponAccuracy;
	UFUNCTION(BlueprintPure)
		const int GetWeaponFireRate() { return FireRate; }
	UFUNCTION(BlueprintPure)
		const float GetWeaponShotRange() { return ShotRange; }
	UFUNCTION(BlueprintPure)
		const float GetWeaponAmmo() { return Ammo; }
	UFUNCTION(BlueprintPure)
		const float GetWeaponReloadingTime() { return ReloadingTime; }
	UFUNCTION(BlueprintPure)
		const float GetWeaponDamage() { return Damage; }
	UFUNCTION(BlueprintPure)
		const float GetWeaponCooldownBetweenFire() { return 60 / FireRate; }

	/* Return Accuracy from 1 - 100 */
	UFUNCTION(BlueprintCallable)
		float CalculateWeaponAccuracyAtDistance(float Distance) const;
};
