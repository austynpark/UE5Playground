// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DWGameState.generated.h"

class UDWExperienceManagerComponent;

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API ADWGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ADWGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY()
	TObjectPtr<UDWExperienceManagerComponent> ExperienceManagerComponent;
};
