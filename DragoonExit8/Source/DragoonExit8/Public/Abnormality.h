// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abnormality.generated.h"

UCLASS(Blueprintable, Abstract)
class DRAGOONEXIT8_API AAbnormality : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Abnormality")
	void OnActivate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Abnormality")
	void OnDeactivate();
};
