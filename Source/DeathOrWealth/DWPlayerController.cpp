// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerController.h"
#include "DWPlayerState.h"

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

ADWPlayerState* ADWPlayerController::GetDWPlayerState() const
{
	return CastChecked<ADWPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UAbilitySystemComponent* ADWPlayerController::GetAbilitySystemComponent() const
{
	ADWPlayerState* PS = GetDWPlayerState();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

