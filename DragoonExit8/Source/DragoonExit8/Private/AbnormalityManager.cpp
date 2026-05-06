// Fill out your copyright notice in the Description page of Project Settings.

#include "AbnormalityManager.h"
#include "SpawnEnvManager.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogDragoonExit8, Log, All);

#define ABNORM_LOG(Verbosity, Color, Duration, Format, ...) \
	{ \
		UE_LOG(LogDragoonExit8, Verbosity, Format, ##__VA_ARGS__); \
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(Format, ##__VA_ARGS__)); \
	}

AAbnormalityManager::AAbnormalityManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAbnormalityManager::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnEnvManager)
	{
		SpawnEnvManager->OnPlayerWentForward.AddDynamic(this, &AAbnormalityManager::OnPlayerWentForward);
		SpawnEnvManager->OnPlayerWentBackward.AddDynamic(this, &AAbnormalityManager::OnPlayerWentBackward);
		ABNORM_LOG(Display, FColor::White, 5.f,
			TEXT("[AbnormalityManager] Initialized | Pool: %d abnormalities | RunCount: 0"),
			AbnormalityList.Num());
	}
	else
	{
		ABNORM_LOG(Warning, FColor::Red, 8.f,
			TEXT("[AbnormalityManager] No SpawnEnvManager assigned — traversal events will not fire"));
	}

	StartNewRun();
}

void AAbnormalityManager::StartNewRun()
{
	if (AbnormalityList.IsEmpty())
	{
		ABNORM_LOG(Display, FColor::Silver, 4.f,
			TEXT("[AbnormalityManager] StartNewRun | AbnormalityList is empty, nothing to activate"));
		return;
	}

	// One extra slot in the range represents "nothing happens".
	int32 Roll = FMath::RandRange(0, AbnormalityList.Num());
	if (Roll < AbnormalityList.Num())
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		CurrentAbnormality = GetWorld()->SpawnActor<AAbnormality>(AbnormalityList[Roll], GetActorTransform(), Params);
		ABNORM_LOG(Warning, FColor::Orange, 6.f,
			TEXT("[AbnormalityManager] Abnormality ACTIVE: '%s' (roll %d/%d) | RunCount: %d"),
			CurrentAbnormality ? *CurrentAbnormality->GetClass()->GetName() : TEXT("SpawnFailed"),
			Roll, AbnormalityList.Num(), RunCount);
		if (CurrentAbnormality)
			CurrentAbnormality->OnActivate();
	}
	else
	{
		ABNORM_LOG(Display, FColor::Silver, 4.f,
			TEXT("[AbnormalityManager] No abnormality this run (roll %d/%d) | RunCount: %d"),
			Roll, AbnormalityList.Num(), RunCount);
	}
}

void AAbnormalityManager::EndCurrentAbnormality()
{
	if (CurrentAbnormality)
	{
		ABNORM_LOG(Warning, FColor::Orange, 4.f,
			TEXT("[AbnormalityManager] Deactivating: '%s'"),
			*CurrentAbnormality->GetClass()->GetName());
		CurrentAbnormality->OnDeactivate();
		CurrentAbnormality = nullptr;
	}
}

void AAbnormalityManager::OnPlayerWentForward()
{
	bool bHadAbnormality = (CurrentAbnormality != nullptr);
	int32 OldCount = RunCount;
	EndCurrentAbnormality();

	if (bHadAbnormality)
	{
		RunCount = 0;
		ABNORM_LOG(Warning, FColor::Yellow, 8.f,
			TEXT("[AbnormalityManager] FORWARD through abnormality | RunCount: %d -> 0 (RESET)"),
			OldCount);
	}
	else
	{
		RunCount = FMath::Min(RunCount + 1, 8);
		ABNORM_LOG(Warning, FColor::Yellow, 8.f,
			TEXT("[AbnormalityManager] FORWARD, no abnormality | RunCount: %d -> %d"),
			OldCount, RunCount);
	}

	StartNewRun();
}

void AAbnormalityManager::OnPlayerWentBackward()
{
	bool bHadAbnormality = (CurrentAbnormality != nullptr);
	int32 OldCount = RunCount;
	EndCurrentAbnormality();

	if (bHadAbnormality)
	{
		RunCount = FMath::Min(RunCount + 1, 8);
		ABNORM_LOG(Warning, FColor::Yellow, 8.f,
			TEXT("[AbnormalityManager] BACKWARD through abnormality | RunCount: %d -> %d"),
			OldCount, RunCount);
	}
	else
	{
		ABNORM_LOG(Display, FColor::Silver, 4.f,
			TEXT("[AbnormalityManager] BACKWARD, no abnormality | RunCount: %d (unchanged)"),
			RunCount);
	}

	StartNewRun();
}

#undef ABNORM_LOG
