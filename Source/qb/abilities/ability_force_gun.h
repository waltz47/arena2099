// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilities/Ability.h"
#include "ability_force_gun.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Aability_force_gun : public AAbility
{
	GENERATED_BODY()
	

public:
	Aability_force_gun();

	virtual void ActivateAbility() override;

	virtual void DeactivateAbility() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float range = 900.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float force_applied = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float damage = 60.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float reduc_factor = 5.f;

};
