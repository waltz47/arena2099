// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilities/Ability.h"
#include "ability_smoke_evade.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Aability_smoke_evade : public AAbility
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	Aability_smoke_evade();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float dt) override;

	virtual void ActivateAbility() override;

	virtual void DeactivateAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float duration = 15.f;

	UPROPERTY()
		FTimerHandle smokehandle;
};
