// Fill out your copyright notice in the Description page of Project Settings.


#include "projectiles/bullet.h"
#include "qb_worldcharacter.h"
#include "kismet/gameplaystatics.h"
#include "defs.h"
#include "qbplayer.h"

Abullet::Abullet() {

}

void Abullet::BeginPlay() {
	Super::BeginPlay();

}

void Abullet::Tick(float dt) {
	Super::Tick(dt);

	if (is_active) {
		AddActorWorldOffset(GetActorForwardVector() * dt * projectileSpeed);
		overlapping.Empty();
		GetOverlappingActors(overlapping, AActor::StaticClass());
		//if (IsValid(shot_by) && overlapping.Find(shot_by) != INDEX_NONE) overlapping.Remove(shot_by);
		if (overlapping.Num() == 1 && overlapping[0] == shot_by) {
			return;
		}
		if (overlapping.Num()) {
			//prints("overlapping %d", overlapping.Num());
			float dmg = UGameplayStatics::ApplyDamage(overlapping[0], projectileDamage, shot_by_controller, shot_by, UDamageType::StaticClass());
			/*
			if (dmg && UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == shot_by) {
				Aqbplayer* pl = Cast<Aqbplayer>(shot_by);
				if (IsValid(pl)) {
					pl->DamageNumbers(dmg, GetActorLocation());
				}
			}*/
			OnBulletEnd();
			is_active = false;
			Destroy();
		}
	}
}

void Abullet::Explode() {
	Super::Explode();
	overlapping.Empty();
	GetOverlappingActors(overlapping, AActor::StaticClass());
	//if (IsValid(shot_by) && overlapping.Find(shot_by) != INDEX_NONE) overlapping.Remove(shot_by);
	if (overlapping.Num() == 1 && overlapping[0] == shot_by) {
		return;
	}
	if (overlapping.Num()) {
		//prints("overlapping %d", overlapping.Num());
		float dmg = UGameplayStatics::ApplyDamage(overlapping[0], projectileDamage, shot_by_controller, shot_by, UDamageType::StaticClass());
		/*
		if (dmg && UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == shot_by) {
			Aqbplayer* pl = Cast<Aqbplayer>(shot_by);
			if (IsValid(pl)) {
				pl->DamageNumbers(dmg, GetActorLocation());
			}
		}*/
		OnBulletEnd();
	}
	is_active = false;
	Destroy();
}
