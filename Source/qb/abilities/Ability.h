// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "gameframework/actor.h"
#include "Components/StaticMeshComponent.h"
#include "Ability.generated.h"


UCLASS()
class QB_API AAbility : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AAbility();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		class Aqb_worldcharacter* owning_gladiator = nullptr;

	UPROPERTY()
		class AController* owning_controller = nullptr;

	UPROPERTY(EditAnywhere)
		float cooldown = 2.f;

	bool is_ability_active = 0;
	float last_used = cooldown/2.f;

	UFUNCTION(BlueprintCallable)
		virtual bool IsAbilityOffCooldown();

	UFUNCTION(BlueprintCallable)
		virtual bool IsAbilityActive();

	UFUNCTION(BlueprintCallable)
		virtual void ActivateAbility();

	UFUNCTION(BlueprintCallable)
		virtual void DeactivateAbility();


	UFUNCTION(BlueprintImplementableEvent)
		void OnActivate();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUse();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDeactivate();

	float added_damage = 0.f;



	

		
};
