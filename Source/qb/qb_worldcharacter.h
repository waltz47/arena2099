// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "defs.h"
#include "abilities/ability.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "environmentquery/envquerytest.h"
#include "qb_worldcharacter.generated.h"

#define inf 10000000

UCLASS()
class QB_API Aqb_worldcharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aqb_worldcharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")  FString gladiator_name = "gladiator";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")  float hp = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")  float max_hp = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")  float armor = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")  float crit_chance = 0.08f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")  float crit_percent = 0.17f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")  float xp = 20.f;
	FRandomStream rng;

	virtual float TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator);

	virtual void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
		void BP_Death();

	UFUNCTION(BlueprintImplementableEvent)
		void DamageNumbers(float dmg);

	UFUNCTION(BlueprintCallable)
		void OnSeeEnemy(Aqb_worldcharacter* char_);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		Aqb_worldcharacter* enemy = nullptr;

	UPROPERTY()
		class AAIController* aicontroller = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UEnvQuery* eqs_MoveTo = nullptr;

	void OnMoveToEQSCompleted(TSharedPtr<FEnvQueryResult> result);
	bool MoveToEqsRunning = false;
	FVector move_to_loc = FVector::ZeroVector;
	float last_move_to_eqs_at = -inf;
	FTimerHandle move_to_eqs_handle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float MoveToEqsTimer = 3.f;

	/***********************************ABILITIES******************************/

	void FindAbilityWithTag();

	UPROPERTY(BlueprintReadOnly)
		AAbility* minigun = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AAbility* missile_launcher = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AAbility* laser = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AAbility* rifle = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AAbility* smoke_evade = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AAbility* force_gun = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AAbility* shield = nullptr;

	UPROPERTY(BlueprintReadOnly)
		AAbility* splitter = nullptr;

	//DASH

	float last_dash_at = -inf;
	bool is_dashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") float dash_cooldown = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") float dash_dist = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") float dash_speed = 800.f;
	bool CanDash();
	void StartDash();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_Dash();
	UFUNCTION(BlueprintImplementableEvent)
		void BP_StopDash();

	void EndDash();
	virtual TArray<FVector> GetOptimalDashPoint();
	void MoveToEQS();

	void Heal(float h) {
		hp += h;
		if (hp > max_hp) {
			hp = max_hp;
		}
	}

	virtual void Gun_Fired_CB() {} //only for player


};
