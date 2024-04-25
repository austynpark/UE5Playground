// Fill out your copyright notice in the Description page of Project Settings.


#include "DWInputConfig.h"

UDWInputConfig::UDWInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UDWInputConfig::FindNativeInputActionWithInputTag(const FGameplayTag& inputTag) const
{
	for (const FDWInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == inputTag))
		{
			return Action.InputAction;
		}
	}

    return nullptr;
}

const UInputAction* UDWInputConfig::FindAbilityInputActionWithInputTag(const FGameplayTag& inputTag) const
{
	for (const FDWInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == inputTag))
		{
			return Action.InputAction;
		}
	}

    return nullptr;
}
