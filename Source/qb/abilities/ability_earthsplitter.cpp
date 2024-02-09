// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/ability_earthsplitter.h"
#include "env.h"
#include "projectiles/earthsplit.h"
#include "kismet/gameplaystatics.h"

Aability_earthsplitter::Aability_earthsplitter() {

}

void Aability_earthsplitter::ActivateAbility() {
	Super::ActivateAbility();
	if (GetWorld()->GetTimerManager().IsTimerActive(split_timer)) {
		return;
	}
	if (!splitter_type) {
		return;
	}
	reachedPoint = GetActorLocation();
	currSpawned = 0;
	target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetWorld()->GetTimerManager().SetTimer(split_timer, this, &Aability_earthsplitter::SpawnSplitter, 1.f / spawnrate, true, -1.f);


}

void Aability_earthsplitter::DeactivateAbility() {
	Super::DeactivateAbility();

	GetWorld()->GetTimerManager().ClearTimer(split_timer);
}

void Aability_earthsplitter::SpawnSplitter() {
	if (!IsValid(target)) {
		DeactivateAbility();
	}
	currSpawned++;
	bool isg = Uenv::IsGroundAt(GetWorld(), reachedPoint);
	if (!isg) {
		DeactivateAbility();
		return;
	}
	reachedPoint = Uenv::GetGroundAt(GetWorld(), reachedPoint);
	FVector movedir = (target->GetActorLocation() - reachedPoint);
	movedir.Normalize();
	FActorSpawnParameters spawn_params;
	spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* actor = GetWorld()->SpawnActor<AActor>(splitter_type, reachedPoint, movedir.Rotation(), spawn_params);
	Aearthsplit* es = Cast<Aearthsplit>(actor);
	if (IsValid(es)) {
		es->player = target;
	}
	if (FVector::Distance(reachedPoint, target->GetActorLocation()) <= endWhenDistLessThan) {
		DeactivateAbility();
		return;
	}
	reachedPoint += movedir * separation;
	
	if (currSpawned >= NumTrails) {
		DeactivateAbility();
		return;
	}

}