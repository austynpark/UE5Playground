// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

/**
 * 
 */
struct FDWGameplayTags
{
public:
	static FDWGameplayTags& Get() { return GameplayTags; }
	static void InitializeDWGameplayTags();
	
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(UGameplayTagsManager& Manager, FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	//static void AddMovementTag(UGameplayTagsManager& manager, FGameplayTag& tag, const ANSICHAR* TagName, )

	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;
	FGameplayTag InputTag_Crouch;
	FGameplayTag InputTag_Slide;

	FGameplayTag Status_Crouching;
	//FGameplayTag Status_

	// Initialization states for the GameFrameworkComponentManager, these are registered in order by DWGameInstance and some actors will skip right to GameplayReady
	/** Actor/component has initially spawned and can be extended */
	FGameplayTag InitState_Spawned;

	/** All required data has been loaded/replicated and is ready for initialization */
	FGameplayTag InitState_DataAvailable;

	/** The available data has been initialized for this actor/component, but it is not ready for full gameplay */
	FGameplayTag InitState_DataInitialized;

	/** The actor/component is fully ready for active gameplay */
	FGameplayTag InitState_GameplayReady;

private:
	static FDWGameplayTags GameplayTags;
};
