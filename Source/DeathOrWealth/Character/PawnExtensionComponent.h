// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "Delegates/Delegate.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Templates/Casts.h"
#include "UObject/NameTypes.h"
#include "UObject/UObjectGlobals.h"

#include "PawnExtensionComponent.generated.h"

class UDWPawnData;
class UDWAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	// GameFeatureName
	static const FName NAME_ActorFeatureName;
	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Lyra|Pawn")
	static UPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPawnExtensionComponent>() : nullptr); }


	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	/** Gets the current ability system component, which may be owned by a different actor */
	UFUNCTION(BlueprintPure, Category = "Lyra|Pawn")
	UDWAbilitySystemComponent* GetDWAbilitySystemComponent() const { return AbilitySystemComponent; }


	/** Sets the current pawn data */
	void SetPawnData(const UDWPawnData* InPawnData);

	/** Should be called by the owning pawn to become the avatar of the ability system. */
	void InitializeAbilitySystem(UDWAbilitySystemComponent* InASC, AActor* InOwnerActor);

	/** Should be called by the owning pawn to remove itself as the avatar of the ability system. */
	void UninitializeAbilitySystem();

	/** Should be called by the owning pawn when the pawn's controller changes. */
	void HandleControllerChanged();

	/** Should be called by the owning pawn when the player state has been replicated. */
	void HandlePlayerStateReplicated();

	/** Should be called by the owning pawn when the input component is setup. */
	void SetupPlayerInputComponent();
	
	/** Register with the OnAbilitySystemInitialized delegate and broadcast if our pawn has been registered with the ability system component */
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);

	/** Register with the OnAbilitySystemUninitialized delegate fired when our pawn is removed as the ability system's avatar actor */
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

	/**
	* UPawnComponent interfaces
	*/
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	/**
	* IGameFrameworkInitStateInterface
	*/
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void CheckDefaultInitialization() final;
		
protected:
	/** Delegate fired when our pawn becomes the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	/** Delegate fired when our pawn is removed as the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
	
	UPROPERTY(EditInstanceOnly, Category = "DeathOrWealth|Pawn")
	TObjectPtr<const UDWPawnData> PawnData;

	/** Pointer to the ability system component that is cached for convenience. */
	UPROPERTY()
	TObjectPtr<UDWAbilitySystemComponent> AbilitySystemComponent;


};
