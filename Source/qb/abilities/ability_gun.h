// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilities/Ability.h"
#include "projectiles/projectile.h"
#include "ability_gun.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Aability_gun : public AAbility
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	Aability_gun();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float dt) override;

	virtual void ActivateAbility() override;

	virtual void DeactivateAbility() override;

	TArray<USceneComponent*> bullet_spawn_points;

	UFUNCTION(BlueprintImplementableEvent)
		void OnReload();

	UFUNCTION(BlueprintImplementableEvent)
		void OnNonprojectileWeaponHit(FVector loc);

	void Shoot();

	void Reload();

	/***************************************GUN PARAMS*********************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fireRate = 7.f; //per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 clipSize = 30; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float reloadTime = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<Aprojectile> projectileType; //per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool projectileBased = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float gunDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float reduc_factor = 5.f;

	FTimerHandle shoot_handle;
	int32 clip_ammo_left;
	bool is_reloading = false;
	FRandomStream rng;
	
};

