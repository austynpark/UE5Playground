// Fill out your copyright notice in the Description page of Project Settings.
#include "DW_PlayerState.h"

#include "AbilitySystemComponent.h"

ADW_PlayerState::ADW_PlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}