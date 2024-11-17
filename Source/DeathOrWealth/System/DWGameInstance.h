// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UDWGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UDWGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Init() override;
	virtual void Shutdown() override;
};
