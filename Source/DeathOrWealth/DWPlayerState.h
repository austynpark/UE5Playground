// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ModularPlayerState.h"

#include "DWPlayerState.generated.h"

class UAbilitySystemComponent;
class UDWAbilitySystemComponent;
class UDWExperienceDefinition;
class UDWPawnData;


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

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UDWPawnData* InPawnData);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;

	/** Called by Controller when its PlayerState is initially replicated. */
	virtual void ClientInitialize(AController* C) override;

	/** Copy properties which need to be saved in inactive PlayerState */
	virtual void CopyProperties(APlayerState* PlayerState) override;

	/** Called on the server when the owning player has disconnected, by default this method destroys this player state */
	virtual void OnDeactivated() override;
	/** Called on the server when the owning player has reconnected and this player state is added to the active players array */
	virtual void OnReactivated() override;

	//~End of APlayerState interface

	static const FName NAME_DWAbilityReady;
private:
	void OnExperienceLoaded(const UDWExperienceDefinition* CurrentExperience);

protected:

	//UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UDWPawnData> PawnData;

private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "DeathOrWealth|PlayerState")
	TObjectPtr<UDWAbilitySystemComponent> AbilitySystemComponent;

};
