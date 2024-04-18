// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_AssetManager.h"
#include "AbilitySystemGlobals.h"


void UDW_AssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("UDW_AssetManager::InitializeAbilitySystem");
	//FLyraGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
