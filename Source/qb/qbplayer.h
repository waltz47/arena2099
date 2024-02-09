// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "components/wallruncomponent.h"
#include "qb_worldcharacter.h"
#include "components/attributecomponent.h"
#include "qbplayer.generated.h"

UCLASS()
class QB_API Aqbplayer : public Aqb_worldcharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Aqbplayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
		UWallRunComponent* wallruncomp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities") 
		int32 playerDashCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float playerDashReplenishTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float playerDashForce = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float playerDashForceFalling = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float playerJumpForce = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float dilationTime = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float dilationQuant = 0.6f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float grappleCooldown = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float grappleLaunchForce = 1600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		FRotator recoil_added = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float recoil_intensity = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float max_velocity = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float velo_div = 50.f;
	
	UPROPERTY(BlueprintReadOnly)
		int32 dashes_left;

	/*UFUNCTION(BlueprintImplementableEvent)
		void DamageNumbers(float dmg, FVector loc);*/

	FTimerHandle dashTimer; //replenish dash
	FTimerHandle stopDashTimer; //stop dash
	float dashRightVal = -1;
	float dashForwardVal = -1;
	bool jumped_in_air = false;
	float last_grapple_at = -inf;

	void AddDashCharge();

	virtual void OnDeath() override;
	void MoveForward(float v);
	void MoveRight(float v);
	void LookUp(float v);
	void Turn(float v);

	void PrimaryAbilityStart();
	void PrimaryAbilityStop();
	void SecondaryAbilityStart();
	void SecondaryAbilityStop();

	void Dash();
	void StopDash();

	void _Jump();

	void TimeDilation();
	void EndDilation();
	float last_dash_at = -10.f;
	FTimerHandle dilationTimer;

	FRotator rifle_rot;
	FRotator rifle_recoil_pos;

	UPROPERTY(BlueprintReadOnly)
	float current_velocity_score = 0.f;

	UFUNCTION(BlueprintCallable)
		float getVelocityScorePercent() {
		return current_velocity_score / max_velocity;
	}

	void Grapple();
	UFUNCTION(BlueprintImplementableEvent)  void OnGrapple();
	UFUNCTION(BlueprintCallable) bool CanGrapple();

	virtual void Gun_Fired_CB() override;

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnNextBoss(FName diedBoss);

};
