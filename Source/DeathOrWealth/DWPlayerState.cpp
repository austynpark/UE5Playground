// Fill out your copyright notice in the Description page of Project Settings.
#include "DWPlayerState.h"

#include "AbilitySystemComponent.h"

ADWPlayerState::ADWPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ADWPlayerState::GetAbilitySystemComponent() const
{
	return nullptr;
}
