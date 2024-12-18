// Fill out your copyright notice in the Description page of Project Settings.
#include "DWPlayerState.h"

#include "AbilitySystem/Abilities/DWGameplayAbility.h"
#include "AbilitySystem/DWAbilitySystemComponent.h"
#include "AbilitySystem/DWAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "GameModes/DWExperienceManagerComponent.h"
#include "GameModes/DeathOrWealthGameModeBase.h"
#include "Character/PawnExtensionComponent.h"
#include "Character/DWPawnData.h"
#include "GameFramework/Pawn.h"
#include "Components/GameFrameworkComponentManager.h"

#include "Net/Core/PushModel/PushModel.h"
#include "Net/UnrealNetwork.h"
#include "CoreTypes.h"
#include "Delegates/Delegate.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "UObject/NameTypes.h"
#include "UObject/UObjectBaseUtility.h"

const FName ADWPlayerState::NAME_DWAbilityReady("DWAbilitiesReady");

ADWPlayerState::ADWPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UDWAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ADWPlayerState::GetAbilitySystemComponent() const
{
	UE_LOG(LogTemp, Warning, TEXT("GET ABILITY SYSTEM"));
	return GetDWAbilitySystemComponent();
}

void ADWPlayerState::SetPawnData(const UDWPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	//MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;
	
	for (const UDWAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	//UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_DWAbilityReady);

	ForceNetUpdate();
}

void ADWPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADWPlayerState::OnExperienceLoaded(const UDWExperienceDefinition* CurrentExperience)
{
	if (ADeathOrWealthGameModeBase* DWGameMode = GetWorld()->GetAuthGameMode<ADeathOrWealthGameModeBase>())
	{
		if (const UDWPawnData* NewPawnData = DWGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ALyraPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}

void ADWPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	if (GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		UDWExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDWExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}

}

void ADWPlayerState::Reset()
{
	Super::Reset();
}

void ADWPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (UPawnExtensionComponent* PawnExtComp = UPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void ADWPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
}

void ADWPlayerState::OnDeactivated()
{
	Super::OnDeactivated();
}

void ADWPlayerState::OnReactivated()
{
}


