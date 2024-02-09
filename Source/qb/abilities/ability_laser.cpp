// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/ability_laser.h"
#include "defs.h"
#include "kismet/gameplaystatics.h"
#include "kismet/kismetsystemlibrary.h"
#include "qb_worldcharacter.h"
#include "components/scenecomponent.h"

Aability_laser::Aability_laser() {
	PrimaryActorTick.bCanEverTick = true;

}

void Aability_laser::BeginPlay() {
	Super::BeginPlay();
	TArray<UActorComponent*> comps;
	comps = GetComponentsByTag(USceneComponent::StaticClass(), FName(LASER_SPAWN_TAG));
	for (UActorComponent* ac : comps) {
		USceneComponent* sc = Cast<USceneComponent>(ac);
		laser_dir_comp = sc;
	}
	if (!IsValid(laser_dir_comp)) {
		dbg("Laser dir comp invalid. Destroying!");
		Destroy();
	}
}

void Aability_laser::Tick(float dt) {
	Super::Tick(dt);

	if (is_ability_active == false) return;

	if ((GetGameTimeSinceCreation() - last_damage_at) <= damageInterval) {
		return;
	}
	
	FVector laser_start = laser_dir_comp->GetComponentLocation();
	FVector laser_end = laser_start + laser_dir_comp->GetComponentRotation().Vector() * 8000.f;
	TArray<AActor*> ignored;
	ignored.Add(this);
	ignored.Add(owning_gladiator);
	TArray<FHitResult> hits;
	FCollisionQueryParams query_params;
	query_params.AddIgnoredActors(ignored);

	GetWorld()->LineTraceMultiByChannel(hits, laser_start, laser_end, ECollisionChannel::ECC_Camera, query_params);
	for (int i = 0; i < hits.Num(); i++) {
		if (hits[i].bBlockingHit) {
			last_damage_at = GetGameTimeSinceCreation();
			UGameplayStatics::ApplyDamage(hits[i].GetActor(), damage, owning_controller, owning_gladiator, UDamageType::StaticClass());
		}
	}
}



void Aability_laser::ActivateAbility()
{
	Super::ActivateAbility();
	//GetWorld()->GetTimerManager().ClearTimer(laserHandle);
	GetWorld()->GetTimerManager().ClearTimer(offHandle);
	GetWorld()->GetTimerManager().SetTimer(laserHandle, this, &Aability_laser::LaserOn, 0.5f, false, -1);
	GetWorld()->GetTimerManager().SetTimer(offHandle, this, &Aability_laser::DeactivateAbility, 4.f, false, -1);
}

void Aability_laser::DeactivateAbility()
{
	Super::DeactivateAbility();
	GetWorld()->GetTimerManager().ClearTimer(laserHandle);
	GetWorld()->GetTimerManager().ClearTimer(offHandle);
	BP_LaserOff();
	dbg("Laser Deactivated");
}


void Aability_laser::LaserOn() {
	dbg("Laser Activated");
	BP_LaserOn();
}