// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DWAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UDWAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	void StartInitialLoading() override;
};
