// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnEnvManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogDragoonExit8, Log, All);

#define SPAWNMGR_LOG(Color, Duration, Format, ...) \
	{ \
		UE_LOG(LogDragoonExit8, Display, Format, ##__VA_ARGS__); \
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(Format, ##__VA_ARGS__)); \
	}

ASpawnEnvManager::ASpawnEnvManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnEnvManager::BeginPlay()
{
	Super::BeginPlay();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	Player = Cast<ADragoonExit8Character>(PlayerPawn);

	if (SpawnEnvNext)
		SpawnEnvNext->OnPlayerEntered.AddDynamic(this, &ASpawnEnvManager::TriggerSwapForward);
	if (SpawnEnvBack)
		SpawnEnvBack->OnPlayerEntered.AddDynamic(this, &ASpawnEnvManager::TriggerSwapBackward);

	SPAWNMGR_LOG(FColor::White, 5.f, TEXT("[SpawnEnvManager] Initialized | Back: %s | Current: %s | Next: %s"),
		SpawnEnvBack    ? *SpawnEnvBack->GetName()    : TEXT("NULL"),
		SpawnEnvCurrent ? *SpawnEnvCurrent->GetName() : TEXT("NULL"),
		SpawnEnvNext    ? *SpawnEnvNext->GetName()    : TEXT("NULL"));
}

void ASpawnEnvManager::TriggerSwapForward()
{
	if (!Player || !SpawnEnvNext || !SpawnEnvCurrent) return;
	SPAWNMGR_LOG(FColor::Green, 4.f, TEXT("[SpawnEnvManager] FORWARD swap -> player hit %s"), *SpawnEnvNext->GetName());
	TeleportPlayerBetweenZones(SpawnEnvNext, SpawnEnvCurrent);
	OnPlayerWentForward.Broadcast();
}

void ASpawnEnvManager::TriggerSwapBackward()
{
	if (!Player || !SpawnEnvBack || !SpawnEnvCurrent) return;
	SPAWNMGR_LOG(FColor::Green, 4.f, TEXT("[SpawnEnvManager] BACKWARD swap -> player hit %s"), *SpawnEnvBack->GetName());
	TeleportPlayerBetweenZones(SpawnEnvBack, SpawnEnvCurrent);
	OnPlayerWentBackward.Broadcast();
}

void ASpawnEnvManager::TeleportPlayerBetweenZones(ASpawnEnv* From, ASpawnEnv* To)
{
	FTransform FromTransform = From->GetActorTransform();
	FTransform ToTransform = To->GetActorTransform();

	FVector LocalPos = FromTransform.InverseTransformPosition(Player->GetActorLocation());
	FVector NewWorldPos = ToTransform.TransformPosition(LocalPos);

	FQuat RotDelta = ToTransform.GetRotation() * FromTransform.GetRotation().Inverse();
	float YawDelta = RotDelta.Rotator().Yaw;

	FVector OldVelocity = Player->GetCharacterMovement()->Velocity;
	FVector NewVelocity = RotDelta.RotateVector(OldVelocity);

	SPAWNMGR_LOG(FColor::Green, 5.f,
		TEXT("[Teleport] %s -> %s | Pos: %s -> %s | Vel: %s -> %s | Yaw+%.1f"),
		*From->GetName(), *To->GetName(),
		*Player->GetActorLocation().ToCompactString(), *NewWorldPos.ToCompactString(),
		*OldVelocity.ToCompactString(), *NewVelocity.ToCompactString(),
		YawDelta);

	if (AController* Controller = Player->GetController())
	{
		FRotator CtrlRot = Controller->GetControlRotation();
		Controller->SetControlRotation(FRotator(CtrlRot.Pitch, CtrlRot.Yaw + YawDelta, CtrlRot.Roll));
	}

	Player->SetActorLocation(NewWorldPos, false, nullptr, ETeleportType::TeleportPhysics);
	Player->GetCharacterMovement()->Velocity = NewVelocity;
}

#undef SPAWNMGR_LOG
