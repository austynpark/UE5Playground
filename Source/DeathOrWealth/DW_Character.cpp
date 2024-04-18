// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_Character.h"

// Sets default values
ADW_Character::ADW_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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

// Called to bind functionality to input
void ADW_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

