// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "DW_PlayerState.generated.h"


class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API ADW_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	// Constructor with FObjectInitalizer https://forums.unrealengine.com/t/constructor-with-or-without-fobjectinitializer/75397/3 
	ADW_PlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//UFUNCTION(BlueprintCallable, Category = "DeathOrWealth|PlayerState")
	//UAbilitySystemComponent* GetDWAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "DeathOrWealth|PlayerState")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

};
