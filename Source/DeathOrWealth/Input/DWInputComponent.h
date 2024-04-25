// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DWInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UDWInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UDWInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

private:
	
};
