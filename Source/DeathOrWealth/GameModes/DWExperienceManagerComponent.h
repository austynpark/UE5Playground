// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "DWExperienceManagerComponent.generated.h"

class UDWExperienceDefinition;

enum class EDWExperienceLoadState
{
	Unloaded,
	Loading,
	Loaded,
	Deactivating
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnExperienceLoaded, const UDWExperienceDefinition*)

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UDWExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UDWExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	bool IsExperienceLoaded() const { return (LoadState == EDWExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }
	const UDWExperienceDefinition* GetCurrentExperienceChecked() const;


	void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate);

#if WITH_SERVER_CODE
	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
#endif

private:

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);

private:

	//TODO: ReplicatedUsing
	UPROPERTY()
	TObjectPtr<const UDWExperienceDefinition> CurrentExperience;
	
	EDWExperienceLoadState LoadState = EDWExperienceLoadState::Unloaded;

	/* Delegate to broadcast after experience loaded */
	FOnExperienceLoaded OnExperienceLoaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
