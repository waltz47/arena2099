// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/ability_smoke_evade.h"

Aability_smoke_evade::Aability_smoke_evade() {
	PrimaryActorTick.bCanEverTick = true;

}

void Aability_smoke_evade::BeginPlay() {
	Super::BeginPlay();
}

void Aability_smoke_evade::Tick(float dt) {
	Super::Tick(dt);

}



void Aability_smoke_evade::ActivateAbility()
{
	Super::ActivateAbility();
	GetWorld()->GetTimerManager().SetTimer(smokehandle, this, &Aability_smoke_evade::DeactivateAbility, duration, false, -1.f);
}

void Aability_smoke_evade::DeactivateAbility()
{
	Super::DeactivateAbility();
	GetWorld()->GetTimerManager().ClearTimer(smokehandle);
}
