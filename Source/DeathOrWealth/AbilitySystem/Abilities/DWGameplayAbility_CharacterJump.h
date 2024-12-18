// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/DWGameplayAbility.h"
#include "UObject/UObjectGlobals.h"

#include "DWGameplayAbility_CharacterJump.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;
/**
 * 
 */
UCLASS(Abstract)
class DEATHORWEALTH_API UDWGameplayAbility_CharacterJump : public UDWGameplayAbility
{
	GENERATED_BODY()
public:
	UDWGameplayAbility_CharacterJump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "DW|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "DW|Ability")
	void CharacterJumpStop();
};
