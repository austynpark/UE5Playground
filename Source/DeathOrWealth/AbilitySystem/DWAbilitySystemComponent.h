// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DWAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UDWAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UDWAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	void TryActivateAbilitiesOnSpawn();
protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
private:
};
