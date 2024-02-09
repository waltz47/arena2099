// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "qb_worldcharacter.h"
#include "boss_mech.generated.h"

UCLASS()
class QB_API Aboss_mech : public Aqb_worldcharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aboss_mech();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator);

	bool AboutToReceiveDamage();

	FVector dashing_to;

	UPROPERTY()
		float time_bw_abilities = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float smoke_evade_heal = 0.3f;

	float last_ability_at = -20.f;

	UFUNCTION(BlueprintImplementableEvent)
		void BP_OnSmokeEvade();

	bool last_dmg_flag = false;

	UFUNCTION(BlueprintImplementableEvent) void On_MinionSpawned(FVector spawnLoc);

	bool CanSpawnMinions();
	void SpawnMinions();
	void OnMinionEQSEnd(TSharedPtr<FEnvQueryResult> result);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") TSubclassOf<AActor> minionClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") int32 maxAllowedMinions = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") int32 num_spawns = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") int32 minion_cooldown = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UEnvQuery* eqs_MinionSpawnPointsEQS = nullptr;
	float last_spawn_at = 0.f;

};
