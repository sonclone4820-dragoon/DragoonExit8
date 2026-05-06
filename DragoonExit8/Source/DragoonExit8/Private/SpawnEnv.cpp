// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnEnv.h"
#include "DragoonExit8Character.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogDragoonExit8, Log, All);

#define SPAWNENV_LOG(Color, Duration, Format, ...) \
	{ \
		UE_LOG(LogDragoonExit8, Display, Format, ##__VA_ARGS__); \
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(Format, ##__VA_ARGS__)); \
	}

ASpawnEnv::ASpawnEnv()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
	CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawnEnv::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASpawnEnv::OnOverlapEnd);
}

void ASpawnEnv::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnEnv::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ADragoonExit8Character* Character = Cast<ADragoonExit8Character>(OtherActor))
	{
		bPlayerInside = true;
		if (!bHasTriggered)
		{
			bHasTriggered = true;
			SPAWNENV_LOG(FColor::Cyan, 3.f, TEXT("[SpawnEnv] '%s' triggered -> broadcasting OnPlayerEntered"), *GetName());
			OnPlayerEntered.Broadcast();
		}
		else
		{
			SPAWNENV_LOG(FColor::Silver, 3.f, TEXT("[SpawnEnv] '%s' entered but already triggered, skipping"), *GetName());
		}
	}
}

void ASpawnEnv::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (ADragoonExit8Character* Character = Cast<ADragoonExit8Character>(OtherActor))
	{
		bPlayerInside = false;
		bHasTriggered = false;
		SPAWNENV_LOG(FColor::Cyan, 3.f, TEXT("[SpawnEnv] '%s' exited -> trigger reset"), *GetName());
	}
}

void ASpawnEnv::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#undef SPAWNENV_LOG
