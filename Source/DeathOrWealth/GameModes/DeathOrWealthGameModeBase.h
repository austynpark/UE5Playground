// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DeathOrWealthGameModeBase.generated.h"

class UDWExperienceDefinition;
class UDWPawnData;

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API ADeathOrWealthGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ADeathOrWealthGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	* AGameModeBase interface
	*/
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override final;
	virtual void InitGameState() override final;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	/**
	* member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	void OnExperienceLoaded(const UDWExperienceDefinition* CurrentExperience);
	const UDWPawnData* GetPawnDataForController(const AController* InController) const;

};
