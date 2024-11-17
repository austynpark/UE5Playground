// Fill out your copyright notice in the Description page of Project Settings.
#include "DWPlayerState.h"

#include "AbilitySystem/DWAbilitySystemComponent.h"

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
