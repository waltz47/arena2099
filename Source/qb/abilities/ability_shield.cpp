// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/ability_shield.h"
#include "defs.h"
#include "qb_worldcharacter.h"

Aability_shield::Aability_shield() {
	PrimaryActorTick.bCanEverTick = true;

}

void Aability_shield::BeginPlay() {
	Super::BeginPlay();


}

void Aability_shield::Tick(float dt) {
	Super::Tick(dt);

	if (IsAbilityActive() && IsValid(owning_gladiator)) {
		owning_gladiator->Heal(healPerTick);
	}

}



void Aability_shield::ActivateAbility()
{
	Super::ActivateAbility();
	GetWorld()->GetTimerManager().ClearTimer(shieldon);
	GetWorld()->GetTimerManager().SetTimer(shieldon, this, &Aability_shield::DeactivateAbility, shieldTime, false, -1);
	if (!IsValid(owning_gladiator)) {
		dbg("No owning gladiator for shield");
		return;
	}
	owning_gladiator->armor += bonusArmor;
}

void Aability_shield::DeactivateAbility()
{
	Super::DeactivateAbility();
	owning_gladiator->armor -= bonusArmor;

}