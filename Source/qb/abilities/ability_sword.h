//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "abilities/Ability.h"
//#include "ability_sword.generated.h"
//
///**
// * 
// */
//UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
//class QB_API Uability_sword : public UAbility
//{
//	GENERATED_BODY()
//	
//public:
//	Uability_sword();
//
//protected:
//	// Called when the game starts
//	virtual void BeginPlay() override;
//
//public:
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//	virtual void ActivateAbility() override;
//
//	FRotator initial_rot = FRotator::ZeroRotator;
//
//	UPROPERTY(EditAnywhere)
//		FRotator attack_rot = FRotator::ZeroRotator;
//
//	UPROPERTY(EditAnywhere)
//		float attackDamage = 10.f;
//
//	UPROPERTY(EditAnywhere)
//		float minDamageInterval = 0.5f;
//
//	bool is_attacking = false;
//	float last_damage_applied_at = -999.f;
//
//};
