// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSetBase.h"
#include <GameplayEffectExtension.h>
#include <GameplayEffect.h>
#include <UnrealMathUtility.h>

UAttributeSetBase::UAttributeSetBase()
	:Health(200), MaxHealth(200)
{

}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(), GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		Health.SetBaseValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		UE_LOG(LogTemp, Warning, TEXT("took some damage %f"), Health.GetCurrentValue());
		OnHealtChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
}
