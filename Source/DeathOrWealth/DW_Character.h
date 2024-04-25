// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "DW_Character.generated.h"

class ADW_PlayerState;

UCLASS()
class DEATHORWEALTH_API ADW_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADW_Character(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	//virtual void RemoveCharacterAbilities();
	
	virtual void InitializePlayerInput();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	ADW_PlayerState* GetDW_PlayerState() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
