// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerController.h"
#include "DWPlayerState.h"
#include "AbilitySystem/DWAbilitySystemComponent.h"

ADWPlayerController::ADWPlayerController(const FObjectInitializer& ObjectInitializer)
{
	//PlayerCameraManagerClass;
}

void ADWPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADWPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void ADWPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ADWPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ADWPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ADWPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ADWPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UDWAbilitySystemComponent* DeathWealthASC = GetDWAbilitySystemComponent())
	{
		DeathWealthASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ADWPlayerState* ADWPlayerController::GetDWPlayerState() const
{
	return CastChecked<ADWPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UDWAbilitySystemComponent* ADWPlayerController::GetDWAbilitySystemComponent() const
{
	ADWPlayerState* PS = GetDWPlayerState();
	return PS ? PS->GetDWAbilitySystemComponent() : nullptr;
}

