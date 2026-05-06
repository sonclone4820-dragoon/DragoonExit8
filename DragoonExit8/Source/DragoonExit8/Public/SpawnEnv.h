// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnEnv.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnteredSpawnEnv);

UCLASS()
class DRAGOONEXIT8_API ASpawnEnv : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnEnv();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool bIsBackEnv = false;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayerInside = false;

	UPROPERTY(BlueprintReadOnly)
	bool bHasTriggered = false;

	UPROPERTY(BlueprintReadOnly)
	FVector ExitDirection = FVector(1.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintAssignable, Category = "SpawnEnv")
	FOnPlayerEnteredSpawnEnv OnPlayerEntered;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
