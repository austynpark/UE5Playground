// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HeroComponent.h"

#include "Character/DWCharacter.h"
#include "Character/PawnExtensionComponent.h"
#include "Character/DWPawnData.h"
#include "Input/DWInputComponent.h"
#include "Input/DWInputConfig.h"
#include "InputActionValue.h"
#include "AbilitySystem/DWAbilitySystemComponent.h"
#include "System/DWGameplayTags.h"
#include "DWPlayerState.h"
#include "DWPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Components/GameFrameworkComponentManager.h"

#include "EnhancedInputSubsystems.h"

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

const FName UHeroComponent::NAME_ActorFeatureName("Hero");

UHeroComponent::UHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FDWGameplayTags& InitTags = FDWGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<ADWPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			ADWPlayerController* PC = GetController<ADWPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !PC || !PC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Wait for player state and extension component
		ADWPlayerState* PS = GetPlayerState<ADWPlayerState>();

		return PS && Manager->HasFeatureReachedInitState(Pawn, UPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}
	else if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void UHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FDWGameplayTags& InitTags = FDWGameplayTags::Get();
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ADWPlayerState* PS = GetPlayerState<ADWPlayerState>();
		if (!ensure(Pawn && PS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UDWPawnData* PawnData = nullptr;
		
		if (UPawnExtensionComponent* PawnExtComp = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UDWPawnData>();

			// TODO(After Ability)
			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			//PawnExtComp->InitializeAbilitySystem(PS->GetDWAbilitySystemComponent(), PS);
		}

		if (ADWPlayerController* DWPC = GetController<ADWPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		//TODO(After Ability)
		//if (bIsLocallyControlled && PawnData)
		//{
		//	if (UDWCameraComponent* CameraComponent = UDWCameraComponent::FindCameraComponent(Pawn))
		//	{
		//		CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
		//	}
		//}
	}
}

void UHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UPawnExtensionComponent::NAME_ActorFeatureName)
	{
		const FDWGameplayTags& InitTags = FDWGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UHeroComponent::CheckDefaultInitialization()
{
	const FDWGameplayTags& InitTags = FDWGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogTemp, Error, TEXT("[UHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("HeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("HeroComponent");

			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));

			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

void UHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(FDWGameplayTags::Get().InitState_Spawned));
	CheckDefaultInitialization();
}

void UHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	UDWInputComponent* DWInput = Cast<UDWInputComponent>(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = Cast<ULocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (!DefaultMappingContext.IsNull())
			Subsystem->AddMappingContext(DefaultMappingContext.LoadSynchronous(), 0);

		if (UPawnExtensionComponent* PawnExtComp = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (const UDWPawnData* PawnData = PawnExtComp->GetPawnData<UDWPawnData>())
			{
				if (PawnData->InputConfig)
				{
					const FDWGameplayTags& GameplayTags = FDWGameplayTags::Get();
					DWInput->BindNativeAction(PawnData->InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);

					//DWInput->BindAbilityActions(InputConfig, this, &ThisClass::)
					// Get InputConfig

					// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
					//Input->BindAction(AimingInputAction, ETriggerEvent::Triggered, this, &AFooBar::SomeCallbackFunc);
				}
			}
		}
	}
}

void UHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	APawn* Pawn = GetPawn<APawn>();

	if (UPawnExtensionComponent* PawnExtComp = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UDWAbilitySystemComponent* ASC = PawnExtComp->GetDWAbilitySystemComponent())
		{
			ASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void UHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	APawn* Pawn = GetPawn<APawn>();

	if (UPawnExtensionComponent* PawnExtComp = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UDWAbilitySystemComponent* ASC = PawnExtComp->GetDWAbilitySystemComponent())
		{
			ASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn->GetController();

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f) // Modifier Swizzle returns non-zero value
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f) // Modifier Swizzle returns non-zero value
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (ADWCharacter* Character = GetPawn<ADWCharacter>())
	{
		Character->ToggleCrouch();
	}
}

void UHeroComponent::Input_Slide(const FInputActionValue& ActionValue)
{
}
