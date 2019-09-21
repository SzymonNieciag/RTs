// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameplayStatic.h"
#include <GameFramework/Actor.h>
#include "Interface/CanBeDamaged.h"

float URTSGameplayStatic::ApplyDamage(AActor * DamagedActor, float BaseDamage, AActor * DamageCauser)
{
	if (DamagedActor && (BaseDamage != 0.f))
	{
		if (ICanBeDamaged* TheInterface = Cast<ICanBeDamaged>(DamagedActor))
		{
			return ICanBeDamaged::Execute_TakeRTSDamage(DamagedActor, BaseDamage, DamageCauser);
		}
	}
	return 0.f;
}