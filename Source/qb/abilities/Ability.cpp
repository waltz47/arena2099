// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/Ability.h"
#include "defs.h"
#include "qb_worldcharacter.h"

// Sets default values for this component's properties
AAbility::AAbility()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts
void AAbility::BeginPlay()
{
	Super::BeginPlay();

	//owning_gladiator = Cast<AqbCharacter>(GetOwner());
	//if (!IsValid(owning_gladiator)) {
	//	print("owning gladiator is invalid");
	//	Destroy();
	//	return;
	//}
	//
}


// Called every frame
void AAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ...
}

bool AAbility::IsAbilityOffCooldown() {
	return ((GetGameTimeSinceCreation() - last_used) > cooldown) && !IsAbilityActive();
}

bool AAbility::IsAbilityActive() {
	return is_ability_active;
}

void AAbility::ActivateAbility() {
	//if (is_ability_active)
		//print("trying to activate already active ability");

	is_ability_active = true;
	last_used = GetGameTimeSinceCreation();
	OnActivate();
}


void AAbility::DeactivateAbility() {
	//if (!is_ability_active)
		//print("trying to deactivate already inactive ability");

	is_ability_active = false;
	OnDeactivate();
}