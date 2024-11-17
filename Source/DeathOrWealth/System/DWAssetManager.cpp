// Fill out your copyright notice in the Description page of Project Settings.


#include "DWAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "System/DWGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DWAssetManager)

UDWAssetManager::UDWAssetManager() : Super()
{

}

UDWAssetManager& UDWAssetManager::Get()
{
	check(GEngine);

	if (UDWAssetManager* SingleTon = Cast<UDWAssetManager>(GEngine->AssetManager))
		return *SingleTon;

	UE_LOG(LogTemp, Fatal,
		TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project settings);"
			" it must be DWAssetManager"));

	return *NewObject<UDWAssetManager>();
}

void UDWAssetManager::StartInitialLoading()
{
	SCOPED_BOOT_TIMING("UDWAssetManager::InitializeAbilitySystem");
	
	DWGameplayTags::Get().InitializeDWGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}


/*
 * LogAssetLoads�̶�� ������� �����ϸ� True�� ��ȯ�ϴ� �Լ�
 */
bool UDWAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* commandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(commandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

/*
 * �ε��� ������ �����ϱ� ���� �߰��ϴ� �Լ�
 */
void UDWAssetManager::AddLoadedAsset(const UObject* Asset)
{
	// Asset�� Nullptr�� �ƴ϶�� �����߰� , Init���ȿ��� ThreadSafe�� ������� ����
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}

/*
 * ���� �Լ��� ���� ���� ������ synchronous load asset�� ���ʿ��ϰ� �ϴ� ���� ������ Ȯ���ϱ� ����
 * ���� �Լ� �ε��ð� üũ �� �ε��Ϸ�Ǹ� ��ȯ�ϴ� �Լ�
 */
UObject* UDWAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// FSoftObjectPath�� ������ ������θ� �����ϴ� ��ü�� �����Ϳ����� UObject�� ǥ��

	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		// �α� �ð� üũ ���� -> ���� ���� LogAssetLoads�� ����ٿ� �־��ٸ� �ð��� üũ On
		if (ShouldLogAssetLoads())
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);

		// StreamableManager�� ����� �񵿱� ������� �����ϴ� ��ü
		// AssetManager�� �����Ѵٸ� AssetManager�� StreamableManager�� ���� �����ε�
		if (UAssetManager::IsValid())
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);

		// �ƴϸ� FSoftObjectPath�� ���� �ٷ� �����ε� -> �𸮾󿡼��� �����ٰ� �����
		// �׻� StaticLoadObject�ϱ� ���� StaticFindObject�� ���� Ȯ���ϰ� �����ϸ� ��¥ �ε�
		return AssetPath.TryLoad();
	}

	return nullptr;
}