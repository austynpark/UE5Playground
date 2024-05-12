// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DWAbilitySystemComponent.h"
#include "DWAbilitySystemComponent.h"
#include "DWCharacter.h"
#include "DWPlayerController.h"

UDWAbilitySystemComponent::UDWAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UDWAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);
	
	if (bHasNewPawnAvatar)
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
		}
	}

}

void UDWAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
}

void UDWAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
}

void UDWAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
}

void UDWAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}
