// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_GameplayTags.h"

#include "GameplayTagsManager.h"

DW_GameplayTags DW_GameplayTags::GameplayTags;

void DW_GameplayTags::InitializeDWGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

}

void DW_GameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Manager, InputTag_Move, "InputTag.Move", "Move Input");
	AddTag(Manager, InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) Input");
	AddTag(Manager, InputTag_Crouch, "InputTag.Crouch", "Crouch Input");
	AddTag(Manager, InputTag_Slide, "InputTag.Slide", "Slide Input");

}

void DW_GameplayTags::AddTag(UGameplayTagsManager& Manager, FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = Manager.AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

