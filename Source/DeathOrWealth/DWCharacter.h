// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"


#include "DWCharacter.generated.h"

class ADWPlayerState;
class UDWInputConfig;
class UCameraComponent;

UCLASS()
class DEATHORWEALTH_API ADWCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADWCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

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

	ADWPlayerState* GetDWPlayerState() const;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DW|Input")
	TSoftObjectPtr<UInputMappingContext> DefaultMappingContext;
};
