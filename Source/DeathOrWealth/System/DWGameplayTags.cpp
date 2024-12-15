// Fill out your copyright notice in the Description page of Project Settings.


#include "DWGameplayTags.h"

#include "GameplayTagsManager.h"

FDWGameplayTags FDWGameplayTags::GameplayTags;

void FDWGameplayTags::InitializeDWGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

}

void FDWGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Manager, InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(Manager, InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	AddTag(Manager, InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	AddTag(Manager, InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");

	AddTag(Manager, InputTag_Move, "InputTag.Move", "Move Input");
	AddTag(Manager, InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) Input");
	AddTag(Manager, InputTag_Crouch, "InputTag.Crouch", "Crouch Input");
	AddTag(Manager, InputTag_Slide, "InputTag.Slide", "Slide Input");

}

void FDWGameplayTags::AddTag(UGameplayTagsManager& Manager, FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = Manager.AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

