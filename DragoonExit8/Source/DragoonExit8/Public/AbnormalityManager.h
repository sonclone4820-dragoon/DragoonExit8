// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abnormality.h"
#include "AbnormalityManager.generated.h"

class ASpawnEnvManager;

UCLASS()
class DRAGOONEXIT8_API AAbnormalityManager : public AActor
{
	GENERATED_BODY()

public:
	AAbnormalityManager();

	// All possible abnormalities. Each run picks one at random, or nothing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abnormality")
	TArray<TSubclassOf<AAbnormality>> AbnormalityList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abnormality")
	ASpawnEnvManager* SpawnEnvManager;

	// Current run index (0–8). Increases when player retreats with an active abnormality;
	// resets to 0 when player pushes through.
	UPROPERTY(BlueprintReadOnly, Category = "Abnormality")
	int32 RunCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Abnormality")
	AAbnormality* CurrentAbnormality = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	void StartNewRun();
	void EndCurrentAbnormality();

	UFUNCTION()
	void OnPlayerWentForward();

	UFUNCTION()
	void OnPlayerWentBackward();
};
