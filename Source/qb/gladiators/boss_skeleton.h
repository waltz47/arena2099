// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "qb_worldcharacter.h"
#include "boss_skeleton.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Aboss_skeleton : public Aqb_worldcharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	Aboss_skeleton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnAttack();

	float last_attack_at = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float attack_rate = 2.f;

	float attack_range;

	UPROPERTY() AActor* playerenemy = nullptr;

	UPROPERTY(BlueprintReadOnly)
		float maxZ;

	float diffZ;


	
	bool CanAttack();

	UFUNCTION(BlueprintImplementableEvent)
		float GetMeshZ();

};
