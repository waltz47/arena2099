// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilities/Ability.h"
#include "ability_shield.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Aability_shield : public AAbility
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	Aability_shield();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float dt) override;

	virtual void ActivateAbility() override;

	virtual void DeactivateAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float shieldTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float bonusArmor = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float healPerTick = 10.f;
	FTimerHandle shieldon;
};
