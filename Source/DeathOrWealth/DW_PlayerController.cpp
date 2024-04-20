// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_PlayerController.h"
#include "DW_PlayerState.h"

ADW_PlayerController::ADW_PlayerController(const FObjectInitializer& ObjectInitializer)
{
	//PlayerCameraManagerClass;
}

void ADW_PlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADW_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

void ADW_PlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ADW_PlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ADW_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

ADW_PlayerState* ADW_PlayerController::GetDW_PlayerState() const
{
	return CastChecked<ADW_PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UAbilitySystemComponent* ADW_PlayerController::GetAbilitySystemComponent() const
{
	ADW_PlayerState* PS = GetDW_PlayerState();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}


