// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "DWAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class DEATHORWEALTH_API UDWAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	UDWAssetManager();

	static UDWAssetManager& Get();
	static bool ShouldLogAssetLoads();
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);
	void AddLoadedAsset(const UObject* Asset);

	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);


	void StartInitialLoading() override final;

public:
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	FCriticalSection SyncObject;

};

template<typename AssetType>
inline AssetType* UDWAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();

		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load Asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
	}

	return LoadedAsset;
}

template<typename AssetType>
inline TSubclassOf<AssetType> UDWAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();

		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Faileed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass & bKeepInMemory)
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
	}

	return LoadedSubclass;
}
