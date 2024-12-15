// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DWExperienceDefinition.h"
#include "GameFeatureAction.h"

#define UE_DISABLE_OPTIMIZATION
UDWExperienceDefinition::UDWExperienceDefinition(const FObjectInitializer& ObjectInitializer)
{
}

#if WITH_EDITOR
EDataValidationResult UDWExperienceDefinition::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	// Actions
	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Result = CombineDataValidationResults(Result ,Action->IsDataValid(ValidationErrors));
		}
		else
		{
			Result = EDataValidationResult::Invalid;
		}
	}
	
	// Why can't we make subclass from BP?
	// Make sure users didn't subclass from a BP of this (it's fine and expected to subclass once in BP, just not twice)
	//if (!GetClass()->IsNative())
	//{
	//	UClass* ParentClass = GetClass()->GetSuperClass();

	//	// Find the native parent
	//	UClass* FirstNativeParent = ParentClass;
	//	while ((FirstNativeParent != nullptr) && !FirstNativeParent->IsNative())
	//	{
	//		FirstNativeParent = FirstNativeParent->GetSuperClass();
	//	}

	//	if (FirstNativeParent != ParentClass)
	//	{
	//		ValidationErrors.Add(FText::Format(LOCTEXT("ExperienceInheritenceIsUnsupported", "Blueprint subclasses of Blueprint experiences is not currently supported (use composition via ActionSets instead). Parent class was {0} but should be {1}."),
	//			FText::AsCultureInvariant(GetPathNameSafe(ParentClass)),
	//			FText::AsCultureInvariant(GetPathNameSafe(FirstNativeParent))
	//		));
	//		Result = EDataValidationResult::Invalid;
	//	}
	//}

	return Result;
}
#endif //WITH_EDITOR

#if WITH_EDITORONLY_DATA
void UDWExperienceDefinition::UpdateAssetBundleData()
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
#endif // WITH_EDITORONLY_DATA

#define UE_ENABLE_OPTIMIZATION