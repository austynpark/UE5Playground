// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"


#include "DW_Character.generated.h"

class ADW_PlayerState;
class UDWInputConfig;
class UCameraComponent;
class UInputMappingContext;

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

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& ActionValue);
	void Input_LookMouse(const FInputActionValue& ActionValue);
	void Input_Crouch(const FInputActionValue& ActionValue);
	void Input_Slide(const FInputActionValue& ActionValue);

	ADW_PlayerState* GetDW_PlayerState() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DW|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

public:
	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DW|Input")
	TObjectPtr<UDWInputConfig> InputConfig;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DW|Input")
	TWeakObjectPtr<UInputMappingContext> DefaultMappingContext;
};
