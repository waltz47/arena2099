// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "projectiles/projectile.h"
#include "bullet.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Abullet : public Aprojectile
{
	GENERATED_BODY()

public:
	Abullet();

	virtual void BeginPlay() override;
	virtual void Tick(float dt) override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnBulletEnd();

	UPROPERTY()
		TArray<AActor*> overlapping;

	virtual void Explode() override;

};
