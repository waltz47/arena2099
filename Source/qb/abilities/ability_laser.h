// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilities/Ability.h"
#include "ability_laser.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Aability_laser : public AAbility
{
	GENERATED_BODY()

public:
	Aability_laser();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float dt) override;

	virtual void ActivateAbility() override;

	virtual void DeactivateAbility() override;

	void LaserOn();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_LaserOn();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_LaserOff();

	UPROPERTY() FTimerHandle laserHandle;
	UPROPERTY() FTimerHandle offHandle;

	UPROPERTY()
		USceneComponent* laser_dir_comp = nullptr; //simple arrow comp with laser tag to get direction

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damageInterval = 0.1f;

	float last_damage_at = -10000.f;

	
};
