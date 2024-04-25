// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_Character.h"
#include "DW_PlayerState.h"
#include "Input/DWInputComponent.h"

#include "EnhancedInputSubsystems.h"

// Sets default values
ADW_Character::ADW_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UAbilitySystemComponent* ADW_Character::GetAbilitySystemComponent() const
{
	ADW_PlayerState* PS = GetDW_PlayerState();
	PS->GetAbilitySystemComponent();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

void ADW_Character::InitializePlayerInput()
{
	//InputComponent

}

void ADW_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// create singleton 1st Gameplay Tags
	// Get InputConfig

	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	//Input->BindAction(AimingInputAction, ETriggerEvent::Triggered, this, &AFooBar::SomeCallbackFunc);
}

ADW_PlayerState* ADW_Character::GetDW_PlayerState() const
{
	return CastChecked<ADW_PlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

// Called when the game starts or when spawned
void ADW_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADW_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

