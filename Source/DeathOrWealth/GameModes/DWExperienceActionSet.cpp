// Fill out your copyright notice in the Description page of Project Settings.

#include "DWExperienceActionSet.h"
#include "GameFeatureAction.h"

UDWExperienceActionSet::UDWExperienceActionSet()
{
}

#if WITH_EDITOR
EDataValidationResult UDWExperienceActionSet::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			EDataValidationResult ChildResult = Action->IsDataValid(ValidationErrors);
			Result = CombineDataValidationResults(Result, ChildResult);
		}
		else
		{
			Result = EDataValidationResult::Invalid;
		}
	}

	return Result;
}
#endif

#if WITH_EDITORONLY_DATA
void UDWExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif