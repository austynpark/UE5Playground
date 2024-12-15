// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWExperienceDefinition.generated.h"

class UGameFeatureAction;
class UDWExperienceActionSet;
class UDWPawnData;
/**
 * 
 */
UCLASS(BlueprintType, Const)
class DEATHORWEALTH_API UDWExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UDWExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//~End of UObject interface

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

public:
	// GameFeatureAction to enable
	UPROPERTY(EditDefaultsOnly, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;

	UPROPERTY(EditDefaultsOnly, Category = "GameFeatures to Enable")
	TArray<FString> GameFeaturesToEnable;

	/** The default pawn class to spawn for players */
	//@TODO: Make soft?
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<const UDWPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Category = "Experience Action Set to Enable")
	TArray<TObjectPtr<UDWExperienceActionSet>> ActionSets;


};
