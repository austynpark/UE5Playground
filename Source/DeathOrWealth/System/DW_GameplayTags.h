// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTags.h"

/**
 * 
 */
struct DW_GameplayTags
{
public:
	static DW_GameplayTags& Get() { return GameplayTags; }
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

private:
	static DW_GameplayTags GameplayTags;
};
