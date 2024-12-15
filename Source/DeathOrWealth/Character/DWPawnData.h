// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWPawnData.generated.h"

class UDWInputConfig;

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UDWPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UDWPawnData(const FObjectInitializer& ObjectInitializer);

	/** Pawn Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeathOrWealth|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DW|Input")
	TObjectPtr<UDWInputConfig> InputConfig;
};
