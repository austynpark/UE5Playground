// Copyright Epic Games, Inc. All Rights Reserved.


#include "DeathOrWealthGameModeBase.h"

#include "DWGameState.h"
#include "DWPlayerController.h"
#include "DWPlayerState.h"
#include "Character/DWPawnData.h"
#include "Character/DWCharacter.h"
#include "Character/PawnExtensionComponent.h"

#include "System/DWAssetManager.h"
#include "DWExperienceManagerComponent.h"
#include "DWExperienceDefinition.h"
#include "TimerManager.h"

ADeathOrWealthGameModeBase::ADeathOrWealthGameModeBase(const FObjectInitializer& ObjectInitializer)
{
	GameStateClass = ADWGameState::StaticClass();
	PlayerControllerClass = ADWPlayerController::StaticClass();
	DefaultPawnClass = ADWCharacter::StaticClass();
}

void ADeathOrWealthGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::FindandSetExperienceData);
}

void ADeathOrWealthGameModeBase::InitGameState()
{
	Super::InitGameState();
	
	UDWExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<UDWExperienceManagerComponent>();
	check(ExperienceManagerComp);
	ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

const UDWPawnData* ADeathOrWealthGameModeBase::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const ADWPlayerState* PS = InController->GetPlayerState<ADWPlayerState>())
		{
			if (const UDWPawnData* PawnData = PS->GetPawnData<UDWPawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	UDWExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDWExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const UDWExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		// return UDWAssetManager::Get().GetDefaultPawnData();
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}


UClass* ADeathOrWealthGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController를 활용하여, PawnData로부터 PawnClass를 유도하자
	if (const UDWPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ADeathOrWealthGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UPawnExtensionComponent* PawnExtComp = UPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UDWPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

void ADeathOrWealthGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Delay starting new players until the experience has been loaded
	// (players who log in prior to that will be started by OnExperienceLoaded)
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

//HandleMatchAssignmentIfNotExpectingOne
void ADeathOrWealthGameModeBase::FindandSetExperienceData()
{
	FPrimaryAssetId ExperienceID;

	UWorld* World = GetWorld();
	
	if (!ExperienceID.IsValid())
	{
		ExperienceID = FPrimaryAssetId(FPrimaryAssetType("DWExperienceDefinition"), FName("BP_DefaultExperience"));
	}

	TryLoadingExperience(ExperienceID);
}

//OnMatchAssignmentGiven
void ADeathOrWealthGameModeBase::TryLoadingExperience(FPrimaryAssetId ExperienceId)
{
#if WITH_SERVER_CODE
	if (ExperienceId.IsValid())
	{
		UDWExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDWExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->ServerSetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
#endif
}

bool ADeathOrWealthGameModeBase::IsExperienceLoaded() const
{
	check(GameState);
	UDWExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDWExperienceManagerComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}

void ADeathOrWealthGameModeBase::OnExperienceLoaded(const UDWExperienceDefinition* CurrentExperience)
{
	// Spawn any players that are already attached
	//@TODO: Here we're handling only *player* controllers, but in GetDefaultPawnClassForController_Implementation we skipped all controllers
	// GetDefaultPawnClassForController_Implementation might only be getting called for players anyways
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}
