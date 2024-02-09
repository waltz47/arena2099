// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/ability_gun.h"
#include "qb_worldcharacter.h"
#include "gameframework/actor.h"
#include "defs.h"
#include "kismet/gameplaystatics.h"

Aability_gun::Aability_gun() {
	PrimaryActorTick.bCanEverTick = true;

}

void Aability_gun::BeginPlay() {
	Super::BeginPlay();

	clip_ammo_left = clipSize;
	TArray<UActorComponent*> comps;
	comps = GetComponentsByTag(USceneComponent::StaticClass(), FName(BULLET_SPAWN_TAG));
	for (UActorComponent* ac : comps) {
		USceneComponent* sc = Cast<USceneComponent>(ac);
		bullet_spawn_points.Add(sc);
	}
	rng = FRandomStream(GetUniqueID());
}

void Aability_gun::Tick(float dt) {
	Super::Tick(dt);

}



void Aability_gun::ActivateAbility()
{
	Super::ActivateAbility();
	GetWorld()->GetTimerManager().ClearTimer(shoot_handle);
	GetWorld()->GetTimerManager().SetTimer(shoot_handle, this, &Aability_gun::Shoot, 1.f / fireRate, true, 0.f);
}

void Aability_gun::DeactivateAbility()
{
	Super::DeactivateAbility();
	GetWorld()->GetTimerManager().ClearTimer(shoot_handle);
}

void Aability_gun::Shoot()
{
	if (!projectileType) {
		dbg("invalid projectile type");
		return;
	}
	if (bullet_spawn_points.Num() == 0) {
		dbg("Gun has no bullet spawn points");
		return;
	}
	if (!clip_ammo_left) {
		return;
	}
	OnUse();
	float to_crit = rng.FRandRange(0.f, 1.f);
	if (projectileBased) {
		//SPAWN PROJECTILE
		FActorSpawnParameters spawn_params;
		spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		for(int32 i=0;i<bullet_spawn_points.Num();i++) {
			Aprojectile* projectile = GetWorld()->SpawnActor<Aprojectile>(projectileType, bullet_spawn_points[i]->GetComponentLocation(), bullet_spawn_points[i]->GetComponentRotation(),  spawn_params);
			projectile->shot_by = owning_gladiator;
			projectile->shot_by_controller = owning_controller;
			projectile->enemy = owning_gladiator->enemy;
			if (to_crit < owning_gladiator->crit_chance) {
				projectile->projectileDamage += (owning_gladiator->crit_percent * projectile->projectileDamage);
			}
			projectile->is_active = true;
		}
	}
	else {
		//LINE TRACE
		FHitResult hit;
		FVector start = bullet_spawn_points[0]->GetComponentLocation();
		FVector end = start + bullet_spawn_points[0]->GetComponentRotation().Vector() * 10000.f;
		FCollisionQueryParams params;
		params.AddIgnoredActor(owning_gladiator);
		GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Camera, params);
		if (hit.bBlockingHit) {
			OnNonprojectileWeaponHit(hit.Location);
			float applied_dmg = gunDamage + added_damage;
			if (to_crit < owning_gladiator->crit_chance) {
				applied_dmg += (rng.FRandRange(0.01f, owning_gladiator->crit_percent) * gunDamage);
			}
			float dist = FVector::Distance(GetActorLocation(), hit.GetActor()->GetActorLocation());
			float reduc = (dist / 2000.f) * (reduc_factor / 100.f);
			applied_dmg -= (applied_dmg * reduc);
			UGameplayStatics::ApplyDamage(hit.GetActor(), applied_dmg, owning_controller, owning_gladiator, UDamageType::StaticClass());
		}
	}

	clip_ammo_left--;
	owning_gladiator->Gun_Fired_CB();
	if (clip_ammo_left <= 0) {
		OnReload();
		FTimerHandle reloadhandle;
		GetWorld()->GetTimerManager().SetTimer(reloadhandle, this, &Aability_gun::Reload, reloadTime, false, -1.f);
		DeactivateAbility();
	}

}

void Aability_gun::Reload() {
	clip_ammo_left = clipSize;
}

