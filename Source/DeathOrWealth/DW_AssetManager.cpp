// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_AssetManager.h"
#include "AbilitySystemGlobals.h"

#include "System/DW_GameplayTags.h"

void UDW_AssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("UDW_AssetManager::InitializeAbilitySystem");
	
	DW_GameplayTags::Get().InitializeDWGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
