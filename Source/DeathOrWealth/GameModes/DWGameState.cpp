// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/DWGameState.h"
#include "DWExperienceManagerComponent.h"

ADWGameState::ADWGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UDWExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
