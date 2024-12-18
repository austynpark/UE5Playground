// Fill out your copyright notice in the Description page of Project Settings.


#include "DWAbilitySystemComponent.h"
#include "Abilities/DWGameplayAbility.h"

#include "Character/DWCharacter.h"
#include "DWPlayerController.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

UDWAbilitySystemComponent::UDWAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
	
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		// Notify all abilities that a new pawn avatar has been set
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			UDWGameplayAbility* DWAbilityCDO = CastChecked<UDWGameplayAbility>(AbilitySpec.Ability);

			if (DWAbilityCDO->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced)
			{
				TArray<UGameplayAbility*> Instances = AbilitySpec.GetAbilityInstances();
				for (UGameplayAbility* AbilityInstance : Instances)
				{
					UDWGameplayAbility* DWAbilityInstance = CastChecked<UDWGameplayAbility>(AbilityInstance);
					DWAbilityInstance->OnPawnAvatarSet();
				}
			}
			else
			{
				DWAbilityCDO->OnPawnAvatarSet();
			}
		}

		TryActivateAbilitiesOnSpawn();
	}

}

void UDWAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
PRAGMA_ENABLE_DEPRECATION_WARNINGS
		UE_LOG(LogTemp, Warning, TEXT("Spec Input Pressed: %s"), *Spec.Ability->GetName());
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UDWAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputPress ability task works.
	if (Spec.IsActive())
	{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
PRAGMA_ENABLE_DEPRECATION_WARNINGS
		UE_LOG(LogTemp, Warning, TEXT("Spec Input Released: %s"), *Spec.Ability->GetName());
		// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UDWAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UDWAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UDWAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	//TODO: May need this in future to block input
	
	if (HasMatchingGameplayTag(TAG_Gameplay_AbilityInputBlocked))
	{
		ClearAbilityInput();
		return;
	}

	static TArray<FGameplayAbilitySpecHandle> abilitiesToActivate;
	abilitiesToActivate.Reset();

	for (const FGameplayAbilitySpecHandle& specHandle : InputPressedSpecHandles)
	{
		FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle);
		if (abilitySpec)
		{
			if (abilitySpec->Ability)
			{
				abilitySpec->InputPressed = true;
				if (!abilitySpec->IsActive())
				{
					const UDWGameplayAbility* abilityCDO = Cast<UDWGameplayAbility>(abilitySpec->Ability);
					if (abilityCDO->ActivationPolicy == EDWAbilityActivationPolicy::OnInputTriggered)
					{
						abilitiesToActivate.Add(specHandle);
					}
				}
				else // is active
				{
					AbilitySpecInputPressed(*abilitySpec);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& specHandle : InputHeldSpecHandles)
	{
		FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle);
		if (abilitySpec)
		{
			if (abilitySpec->Ability && !abilitySpec->IsActive())
			{
				const UDWGameplayAbility* abilityCDO = Cast<UDWGameplayAbility>(abilitySpec->Ability);
				if (abilityCDO->ActivationPolicy == EDWAbilityActivationPolicy::WhileInputActive)
				{
					abilitiesToActivate.Add(specHandle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& specHandle : abilitiesToActivate)
	{
		TryActivateAbility(specHandle);
	}

	for (const FGameplayAbilitySpecHandle& specHandle : InputReleasedSpecHandles)
	{
		FGameplayAbilitySpec* abilitySpec = FindAbilitySpecFromHandle(specHandle);
		if (abilitySpec)
		{
			if (abilitySpec->Ability)
			{
				abilitySpec->InputPressed = false;
				if (abilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*abilitySpec);
				}
			}
		}
	}


	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UDWAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UDWAbilitySystemComponent::TryActivateAbilitiesOnSpawn()
{
	ABILITYLIST_SCOPE_LOCK();
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		const UDWGameplayAbility* DWAbilityCDO = CastChecked<UDWGameplayAbility>(AbilitySpec.Ability);
		DWAbilityCDO->TryActivateAbilityOnSpawn(AbilityActorInfo.Get(), AbilitySpec);
	}
}
