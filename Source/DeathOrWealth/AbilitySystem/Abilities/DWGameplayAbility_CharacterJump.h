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

	UFUNCTION(BlueprintCallable, Category = "DW|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "DW|Ability")
	void CharacterJumpStop();
};
