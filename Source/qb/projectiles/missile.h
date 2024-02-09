// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "projectiles/projectile.h"
#include "missile.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Amissile : public Aprojectile
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float dt) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float radius = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float speed = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float max_altitude = 3600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float no_homing_at = 1000.f;

	FVector last_known_pos;

	bool moving_up = 1;
	FVector epoch;
	FVector towards_down = FVector::ZeroVector;

	UFUNCTION(BlueprintImplementableEvent)
		void OnExplode();

	virtual void Explode() override;


};
