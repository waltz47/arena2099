// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "qb_worldcharacter.h"
#include "defs.h"
#include "abilities/ability.h"
#include "projectiles/projectile.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "environmentquery/envquerytest.h"
//#include "myworker.h"
//#include "HAL/Runnable.h"
#include "boss_wizard.generated.h"


UENUM(BlueprintType)
enum class eWizardPhase :uint8 {
	PHASE_I,
	PHASE_II,
	PHASEIII
};

/**
 * 
 */
UCLASS()
class QB_API Aboss_wizard : public Aqb_worldcharacter
{
	GENERATED_BODY()


public:
	Aboss_wizard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator);

	UPROPERTY(BlueprintReadOnly)
		eWizardPhase phase = eWizardPhase::PHASE_I;

	bool IsAnyAbilityActive();//use only 1 ability at a time
	bool ShieldParamsMet();

	void TryTeleport();
	void OnTeleportEQSOver(TSharedPtr<FEnvQueryResult> result);


	void SpawnMinions();
	void OnMinionEQSEnd(TSharedPtr<FEnvQueryResult> result);
	bool CanSpawnMinions();
	bool CanTeleport();
	void WiTeleportTo();
	UFUNCTION(BlueprintImplementableEvent) void TeleportEffectsOn();
	UFUNCTION(BlueprintImplementableEvent) void TeleportEffectsOff();
	UFUNCTION(BlueprintImplementableEvent) void On_SparkAbilityUsed();
	UFUNCTION(BlueprintImplementableEvent) void On_ShieldAbilityUsed();
	UFUNCTION(BlueprintImplementableEvent) void On_WaterballRainAbilityUsed();
	UFUNCTION(BlueprintImplementableEvent) void On_TeleportAbilityUsed();
	UFUNCTION(BlueprintImplementableEvent) void On_MinionSpawned(FVector spawnLoc);
	UFUNCTION(BlueprintImplementableEvent) void On_SplitterUsed();

	FVector teleport_loc;
	float last_spawn_at = 0.f;
	float last_ability_at = -10.f;
	float last_teleport_at = -999.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") float minion_cooldown = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") float teleport_cooldown = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") int32 num_spawns = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") float ability_spacing = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") TSubclassOf<AActor> minionClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") int32 maxAllowedMinions = 5;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UEnvQuery* eqs_TeleportTo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UEnvQuery* eqs_MinionSpawnPointsEQS = nullptr;

	UPROPERTY(EditANywhere, BlueprintReadWrite)
		TArray<TSubclassOf<Aprojectile>> wizardSpellTypes;


	UFUNCTION(BlueprintCallable)
		void AbiltyUseCalilback(AAbility* ability);

	FTimerHandle th;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool ability_pending = false;


	UPROPERTY()
		TArray<AActor*> spawnedMinions;

	UPROPERTY()
		TArray<bool> canMoveForward;

	//UPROPERTY()
		//FMyWorker* worker;



	
};
