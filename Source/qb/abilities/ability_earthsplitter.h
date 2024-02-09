// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilities/Ability.h"
#include "ability_earthsplitter.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Aability_earthsplitter : public AAbility
{
	GENERATED_BODY()

public:
	Aability_earthsplitter();

	virtual void ActivateAbility() override;

	virtual void DeactivateAbility() override;

	void SpawnSplitter();

	FVector reachedPoint;
	FTimerHandle split_timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 NumTrails = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float separation = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float endWhenDistLessThan = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float spawnrate = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AActor> splitter_type;

	int32 currSpawned = 0;

	UPROPERTY() AActor* target = nullptr;
	
};
