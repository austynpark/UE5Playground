// Copyright Epic Games, Inc. All Rights Reserved.


#include "DeathOrWealthGameModeBase.h"

ADeathOrWealthGameModeBase::ADeathOrWealthGameModeBase(const FObjectInitializer& ObjectInitializer)
{
}

void ADeathOrWealthGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
}

void ADeathOrWealthGameModeBase::InitGameState()
{
}

UClass* ADeathOrWealthGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return nullptr;
}

APawn* ADeathOrWealthGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return nullptr;
}

void ADeathOrWealthGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
}

void ADeathOrWealthGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
}

void ADeathOrWealthGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
}

bool ADeathOrWealthGameModeBase::IsExperienceLoaded() const
{
	return false;
}

void ADeathOrWealthGameModeBase::OnExperienceLoaded(const UDWExperienceDefinition* CurrentExperience)
{
}

const UDWPawnData* ADeathOrWealthGameModeBase::GetPawnDataForController(const AController* InController) const
{
	return nullptr;
}
