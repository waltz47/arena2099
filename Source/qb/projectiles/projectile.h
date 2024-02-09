// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "projectile.generated.h"

UCLASS()
class QB_API Aprojectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aprojectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* sceneRoot = nullptr;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* projectileMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float projectileDamage = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float projectileSpeed = 1500.f;

	int32 dmginstances = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 dmg_instances_to_destroy = 4;

	UPROPERTY()
		AActor* shot_by = nullptr;

	UPROPERTY()
		AController* shot_by_controller = nullptr;

	UPROPERTY()
		AActor* enemy = nullptr;

	bool is_active = false;

	virtual float TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator);

	virtual void Explode() {}

};
