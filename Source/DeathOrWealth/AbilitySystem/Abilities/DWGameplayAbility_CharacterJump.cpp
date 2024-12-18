// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DWGameplayAbility_CharacterJump.h"

#include "Character/DWCharacter.h"

UDWGameplayAbility_CharacterJump::UDWGameplayAbility_CharacterJump(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UDWGameplayAbility_CharacterJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ADWCharacter* LyraCharacter = Cast<ADWCharacter>(ActorInfo->AvatarActor.Get());
	if (!LyraCharacter || !LyraCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UDWGameplayAbility_CharacterJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Stop jumping in case the ability blueprint doesn't call it.
	CharacterJumpStop();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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
		if (Character->IsLocallyControlled() && Character->bPressedJump)
			Character->StopJumping();
	}
}
