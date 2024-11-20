// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWExperienceActionSet.generated.h"

class UGameFeatureAction;

/**
 * Defintion of ActionSet in Experience to perform updating asset bundle data of all the actions included
 */
UCLASS(BlueprintType, NotBlueprintable)
class DEATHORWEALTH_API UDWExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UDWExperienceActionSet();

	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//~End of UObject interface

	//~UPrimaryDataAsset interface
#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	//~End of UPrimaryDataAsset interface

public:
	// List of actions to perform as this experience is loaded/activated/deactivated/unloaded
	UPROPERTY(EditAnywhere, /*Instanced,*/ Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	// List of Game Feature Plugins this experience wants to have active
	UPROPERTY(EditAnywhere, Category = "Feature Dependencies")
	TArray<FString> GameFeaturesToEnable;
};
