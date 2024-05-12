// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DWGameplayAbility.h"

#include "AbilitySystem/DWAbilitySystemComponent.h"
#include "System/DWGameplayTags.h"
#include "DWCharacter.h"
#include "DWPlayerController.h"


UDWGameplayAbility::UDWGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EDWAbilityActivationPolicy::OnInputTriggered;
}

UDWAbilitySystemComponent* UDWGameplayAbility::GetDWAbilitySystemComponentFromActorInfo() const
{
	return CurrentActorInfo ? Cast<UDWAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr;
}

ADWPlayerController* UDWGameplayAbility::GetDWPlayerControllerFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ADWPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr;
}

AController* UDWGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Loop through owner chain and return controller
		AActor* OwnerActor = CurrentActorInfo->OwnerActor.Get();
		while (OwnerActor)
		{
			if (AController* C = Cast<AController>(OwnerActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(OwnerActor))
			{
				return Pawn->GetController();
			}

			OwnerActor = OwnerActor->GetOwner();
		}
	}

	return nullptr;
}

ADWCharacter* UDWGameplayAbility::GetDWCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ADWCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

bool UDWGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	UDWAbilitySystemComponent* ASC = CastChecked<UDWAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UDWGameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{
}

void UDWGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	K2_OnAbilityAdded();
}

void UDWGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	K2_OnAbilityRemoved();

	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UDWGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDWGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDWGameplayAbility::ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const
{
	Super::ApplyAbilityTagsToGameplayEffectSpec(Spec, AbilitySpec);
}

bool UDWGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return false;
}

void UDWGameplayAbility::OnPawnAvatarSet()
{
	K2_OnPawnAvatarSet();
}

