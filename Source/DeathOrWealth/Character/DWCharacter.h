// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "ModularCharacter.h"


#include "DWCharacter.generated.h"

class ADWPlayerState;
class UDWInputConfig;
class UCameraComponent;
class UPawnExtensionComponent;

UCLASS()
class DEATHORWEALTH_API ADWCharacter : public AModularCharacter, public IAbilitySystemInterface
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

	ADWPlayerState* GetDWPlayerState() const;

	void ToggleCrouch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DW|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DW|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPawnExtensionComponent> PawnExtComponent;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DW|Input")
	TObjectPtr<UDWInputConfig> InputConfig;

};
