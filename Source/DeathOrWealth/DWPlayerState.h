// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.h"

#include "DWPlayerState.generated.h"

class UAbilitySystemComponent;
class UDWAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API ADWPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	// Constructor with FObjectInitalizer https://forums.unrealengine.com/t/constructor-with-or-without-fobjectinitializer/75397/3 
	ADWPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "DeathOrWealth|PlayerState")
	UDWAbilitySystemComponent* GetDWAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "DeathOrWealth|PlayerState")
	TObjectPtr<UDWAbilitySystemComponent> AbilitySystemComponent;

};
