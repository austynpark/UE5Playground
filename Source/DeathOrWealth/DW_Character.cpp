// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_Character.h"
#include "DW_PlayerState.h"
#include "Input/DWInputComponent.h"
#include "System/DW_GameplayTags.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"

// Sets default values
ADW_Character::ADW_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->GravityScale = 1.0f;
	MoveComp->MaxAcceleration = 2400.0f;
	MoveComp->BrakingFrictionFactor = 1.0f;
	MoveComp->BrakingFriction = 6.0f;
	MoveComp->GroundFriction = 8.0f;
	MoveComp->BrakingDecelerationWalking = 1400.0f;
	MoveComp->bUseControllerDesiredRotation = false;
	MoveComp->bOrientRotationToMovement = false;
	MoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	MoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->SetCrouchedHalfHeight(65.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(MeshComp); // attaching socket in here doesn't work bc of missing skeletal mesh
	CameraComponent->bUsePawnControlRotation = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f; //?
	CrouchedEyeHeight = 50.0f; //?
}

UAbilitySystemComponent* ADW_Character::GetAbilitySystemComponent() const
{
	ADW_PlayerState* PS = GetDW_PlayerState();
	PS->GetAbilitySystemComponent();
	return PS ? PS->GetAbilitySystemComponent() : nullptr;
}

void ADW_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UDWInputComponent* DWInput = Cast<UDWInputComponent>(PlayerInputComponent);
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	//TODO: may need to use priority for custom user-setting input (PlayerMappableInputConfig)
	if(!DefaultMappingContext.IsNull())
		Subsystem->AddMappingContext(DefaultMappingContext.LoadSynchronous(), 0);

	const DW_GameplayTags& GameplayTags = DW_GameplayTags::Get();
	DWInput->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
	//DWInput->BindAbilityActions(InputConfig, this, &ThisClass::)
	// Get InputConfig

	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	//Input->BindAction(AimingInputAction, ETriggerEvent::Triggered, this, &AFooBar::SomeCallbackFunc);
}

void ADW_Character::Input_Move(const FInputActionValue& ActionValue)
{
	if (Controller)
	{
		const FVector2D Value = ActionValue.Get<FVector2D>();	
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (!FMath::IsNearlyZero(Value.X)) // Modifier Swizzle returns non-zero value
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(MovementDirection, Value.X);
			UE_LOG(LogTemp, Warning, TEXT("Value.x: %f"), Value.X);
		}

		if (!FMath::IsNearlyZero(Value.Y)) // Modifier Swizzle returns non-zero value
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(MovementDirection, Value.Y);
			UE_LOG(LogTemp, Warning, TEXT("Value.y: %f"), Value.Y);
		}
	}
}

void ADW_Character::Input_LookMouse(const FInputActionValue& ActionValue)
{
	const FVector2D Value = ActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

void ADW_Character::Input_Crouch(const FInputActionValue& ActionValue)
{
	const UCharacterMovementComponent* MoveComp = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());

	if (bIsCrouched || MoveComp->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (MoveComp->IsMovingOnGround())
	{
		Crouch();
	}
}

void ADW_Character::Input_Slide(const FInputActionValue& ActionValue)
{
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

