// Copyright Epic Games, Inc. All Rights Reserved.


#include "DeathOrWealthGameModeBase.h"

#include "DWGameState.h"
#include "DWPlayerController.h"
#include "DWPlayerState.h"
#include "Character/DWCharacter.h"

#include "DWExperienceManagerComponent.h"

ADeathOrWealthGameModeBase::ADeathOrWealthGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	GameStateClass = ADWGameState::StaticClass();
	PlayerControllerClass = ADWPlayerController::StaticClass();
	DefaultPawnClass = ADWCharacter::StaticClass();
}

void ADeathOrWealthGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	
}

void ADeathOrWealthGameModeBase::InitGameState()
{
	Super::InitGameState();
	
	UDWExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<UDWExperienceManagerComponent>();
	ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
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

void ADeathOrWealthGameModeBase::FindandSetExperienceData()
{
}

void ADeathOrWealthGameModeBase::TryLoadingExperience(FPrimaryAssetId ExperienceId)
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
