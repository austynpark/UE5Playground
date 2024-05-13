// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DWGameplayAbility_CharacterJump.h"

#include "DWCharacter.h"

UDWGameplayAbility_CharacterJump::UDWGameplayAbility_CharacterJump(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UDWGameplayAbility_CharacterJump::CharacterJumpStart()
{
	if (ADWCharacter* Character = GetDWCharacterFromActorInfo())
	{
		if (Character->IsLocallyControlled() && !Character->bPressedJump)
		{
			if (Character->bIsCrouched)
				Character->UnCrouch();
			else
				Character->Jump();
		}
	}
}

void UDWGameplayAbility_CharacterJump::CharacterJumpStop()
{
	if (ADWCharacter* Character = GetDWCharacterFromActorInfo())
	{
		if (Character->IsLocallyControlled() && !Character->bPressedJump)
			Character->StopJumping();
	}
}
