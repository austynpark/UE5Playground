// Fill out your copyright notice in the Description page of Project Settings.


#include "DWGameplayTags.h"

#include "GameplayTagsManager.h"

DWGameplayTags DWGameplayTags::GameplayTags;

void DWGameplayTags::InitializeDWGameplayTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

}

void DWGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Manager, InputTag_Move, "InputTag.Move", "Move Input");
	AddTag(Manager, InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) Input");
	AddTag(Manager, InputTag_Crouch, "InputTag.Crouch", "Crouch Input");
	AddTag(Manager, InputTag_Slide, "InputTag.Slide", "Slide Input");

}

void DWGameplayTags::AddTag(UGameplayTagsManager& Manager, FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = Manager.AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

