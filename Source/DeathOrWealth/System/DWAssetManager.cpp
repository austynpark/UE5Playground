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
 * LogAssetLoads이라는 명령줄이 존재하면 True를 반환하는 함수
 */
bool UDWAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* commandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(commandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

/*
 * 로딩된 에셋을 관리하기 위해 추가하는 함수
 */
void UDWAssetManager::AddLoadedAsset(const UObject* Asset)
{
	// Asset이 Nullptr이 아니라면 에셋추가 , Init동안에는 ThreadSafe가 보장되지 않음
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}

/*
 * 동기 함수를 따로 만든 이유는 synchronous load asset이 불필요하게 하는 것이 없는지 확인하기 위해
 * 동기 함수 로딩시간 체크 후 로딩완료되면 반환하는 함수
 */
UObject* UDWAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// FSoftObjectPath은 에셋의 참조경로를 저장하는 객체로 에디터에서는 UObject로 표시

	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		// 로깅 시간 체크 응용 -> 만약 내가 LogAssetLoads를 명령줄에 넣었다면 시간초 체크 On
		if (ShouldLogAssetLoads())
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);

		// StreamableManager는 동기와 비동기 입출력을 관리하는 객체
		// AssetManager가 존재한다면 AssetManager의 StreamableManager를 통해 정적로딩
		if (UAssetManager::IsValid())
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);

		// 아니면 FSoftObjectPath를 통해 바로 정적로딩 -> 언리얼에서도 느리다고 명시함
		// 항상 StaticLoadObject하기 전에 StaticFindObject를 통해 확인하고 실패하면 진짜 로딩
		return AssetPath.TryLoad();
	}

	return nullptr;
}