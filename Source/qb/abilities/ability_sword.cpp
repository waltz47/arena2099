//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "abilities/ability_sword.h"
//#include "qbcharacter.h"
//#include "kismet/gameplaystatics.h"
//
//Uability_sword::Uability_sword() {
//
//
//}
//
//void Uability_sword::BeginPlay() {
//	Super::BeginPlay();
//	initial_rot = GetComponentRotation();
//}
//
//void Uability_sword::TickComponent(float dt, ELevelTick tt, FActorComponentTickFunction* tfn) {
//	Super::TickComponent(dt, tt, tfn);
//
//	if (!IsValid(owning_gladiator)) {
//		return;
//	}
//
//	if (is_attacking) {
//		FRotator lerprot = FMath::RInterpTo(GetComponentRotation(), attack_rot, dt, 15.f);
//		if (lerprot.Equals(attack_rot, 5.f)) {
//			is_attacking = false;
//		}
//		
//		SetWorldRotation(lerprot);
//
//		TArray<AActor*> overlapping;
//		GetOverlappingActors(overlapping, AqbCharacter::StaticClass());
//		if (IsValid(GetOwner()) && overlapping.Find(GetOwner()) != INDEX_NONE) overlapping.Remove(GetOwner());
//		if (overlapping.Num() && ((GetOwner()->GetGameTimeSinceCreation() - last_damage_applied_at) > minDamageInterval)) {
//			last_damage_applied_at = GetOwner()->GetGameTimeSinceCreation();
//			UGameplayStatics::ApplyDamage(overlapping[0], attackDamage, owning_gladiator->GetController(), owning_gladiator, UDamageType::StaticClass());
//		}
//
//	}
//	else {
//		FRotator lerprot = FMath::RInterpTo(GetComponentRotation(), initial_rot, dt, 15.f);
//		SetWorldRotation(lerprot);
//	}
//
//}
//
//
//
//void Uability_sword::ActivateAbility()
//{
//	Super::ActivateAbility();
//	is_attacking = true;
//}
