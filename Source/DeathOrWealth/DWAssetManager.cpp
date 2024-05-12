// Fill out your copyright notice in the Description page of Project Settings.


#include "DWAssetManager.h"
#include "AbilitySystemGlobals.h"

#include "System/DWGameplayTags.h"

void UDWAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("UDWAssetManager::InitializeAbilitySystem");
	
	DWGameplayTags::Get().InitializeDWGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
