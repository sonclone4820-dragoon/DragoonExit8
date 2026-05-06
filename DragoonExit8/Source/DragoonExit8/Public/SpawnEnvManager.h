// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnEnv.h"
#include "DragoonExit8Character.h"
#include "SpawnEnvManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerTraversed);

UCLASS()
class DRAGOONEXIT8_API ASpawnEnvManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnEnvManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEnv")
	ASpawnEnv* SpawnEnvBack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEnv")
	ASpawnEnv* SpawnEnvCurrent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnEnv")
	ASpawnEnv* SpawnEnvNext;
	
	ADragoonExit8Character* Player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "SpawnEnv")
	FOnPlayerTraversed OnPlayerWentForward;

	UPROPERTY(BlueprintAssignable, Category = "SpawnEnv")
	FOnPlayerTraversed OnPlayerWentBackward;

	UFUNCTION()
	void TriggerSwapForward();
	UFUNCTION()
	void TriggerSwapBackward();

private:
	void TeleportPlayerBetweenZones(ASpawnEnv* From, ASpawnEnv* To);

};
