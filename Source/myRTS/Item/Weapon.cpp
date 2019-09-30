// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

float AWeapon::CalculateWeaponAccuracyAtDistance(float Distance) const
{
	float DistanceTmp = 0.0f;
	float AccuracyTmp = 0.0f;
	if (Distance > 0 && Distance < WeaponAccuracy.FirstThreshold.Distance)
	{
		return WeaponAccuracy.FirstThreshold.Accuracy;
	}
	else if (Distance > WeaponAccuracy.FirstThreshold.Distance && Distance < WeaponAccuracy.SecoundThreshold.Distance)
	{
		DistanceTmp = WeaponAccuracy.FirstThreshold.Distance + WeaponAccuracy.SecoundThreshold.Distance;
		AccuracyTmp = WeaponAccuracy.FirstThreshold.Accuracy + WeaponAccuracy.SecoundThreshold.Accuracy;
		return Distance / DistanceTmp * AccuracyTmp;
	}
	else if (Distance > WeaponAccuracy.SecoundThreshold.Distance && Distance < WeaponAccuracy.ThirdThreshold.Distance)
	{
		DistanceTmp = WeaponAccuracy.SecoundThreshold.Distance + WeaponAccuracy.ThirdThreshold.Distance;
		AccuracyTmp = WeaponAccuracy.SecoundThreshold.Accuracy + WeaponAccuracy.ThirdThreshold.Accuracy;
		return Distance / DistanceTmp * AccuracyTmp;
	}
	else
	{
		return WeaponAccuracy.ThirdThreshold.Accuracy;
	}
	return 0.0f;
}

