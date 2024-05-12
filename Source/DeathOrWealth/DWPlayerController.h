// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DWPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API ADWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ADWPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "DeathOrWealth|PlayerController")
	ADWPlayerState* GetDWPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "DeathOrWealth|PlayerController")
	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	////~AController interface
	//virtual void OnUnPossess() override;
	////~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~End of APlayerController interface

	////~ACommonPlayerController interface
	//virtual void OnPossess(APawn* InPawn) override;
	////~End of ACommonPlayerController interface

private:
	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

private:
protected:
	//// Called when the player state is set or cleared
	//virtual void OnPlayerStateChanged();

private:
	//void BroadcastOnPlayerStateChanged();

protected:
	////~AController interface
	//virtual void InitPlayerState() override;
	//virtual void CleanupPlayerState() override;
	//virtual void OnRep_PlayerState() override;
	////~End of AController interface

	////~APlayerController interface
	//virtual void SetPlayer(UPlayer* InPlayer) override;
	//virtual void AddCheats(bool bForce) override;

	//virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	//virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;

	//virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	////~End of APlayerController interface
};
