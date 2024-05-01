// Fill out your copyright notice in the Description page of Project Settings.


#include "DWInputComponent.h"

UDWInputComponent::UDWInputComponent(const FObjectInitializer& ObjectInitializer)
{
	
}

void UDWInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
