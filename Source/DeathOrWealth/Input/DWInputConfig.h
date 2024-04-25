// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "DWInputConfig.generated.h"

class UInputAction;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDWInputAction
{
	GENERATED_BODY();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

UCLASS(BlueprintType, Const)
class DEATHORWEALTH_API UDWInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UDWInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "DeathOrWealth|Pawn")
	const UInputAction* FindNativeInputActionWithInputTag(const FGameplayTag& inputTag) const;
	UFUNCTION(BlueprintCallable, Category = "DeathOrWealth|Pawn")
	const UInputAction* FindAbilityInputActionWithInputTag(const FGameplayTag& inputTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDWInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDWInputAction> AbilityInputActions;
};
