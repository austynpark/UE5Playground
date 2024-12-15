// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "DWPlayerState.h"
#include "System/DWGameplayTags.h"
#include "UObject/UnrealNames.h"

const FName UPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UPawnExtensionComponent::UPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UPawnExtensionComponent::SetPawnData(const UDWPawnData* InPawnData)
{
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	PawnData = InPawnData;
	CheckDefaultInitialization();
}

void UPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

/*
* 액터 생성 후 Component로 Attach된 후에 초기화가 되었을 때에 가장 먼저 호출되는 함수.
* 이후에 BeginPlay 호출.
* RegisterInitStateFeature()를 호출하여 ComponentManager에게 Component를 등록
*/
void UPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		return;
	}

	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

void UPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	* NAME_None: Call every actor init state changes other than me.
	*/
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// Notifies state manager that we have spawned, then try rest of default initialization
	ensure(TryToChangeInitState(FDWGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void UPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == FDWGameplayTags::Get().InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FDWGameplayTags& InitTags = FDWGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	const ADWPlayerState* PS = GetPlayerState<ADWPlayerState>();


	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
			return true;
	}

	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// Pawn data is required.
		if (!PawnData)
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		if (bHasAuthority || bIsLocallyControlled)
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}
	else if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
}

void UPawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	const FDWGameplayTags& InitTags = FDWGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}
