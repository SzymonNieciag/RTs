// Copyright 2019 Dan Kestranek.


#include "TSAbilitySystemComponent.h"

void UTSAbilitySystemComponent::ReceiveDamage(UTSAbilitySystemComponent * SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
