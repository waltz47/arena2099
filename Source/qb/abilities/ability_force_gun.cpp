// Fill out your copyright notice in the Description page of Project Settings.


#include "abilities/ability_force_gun.h"
#include "defs.h"
//#include "gameframework/character.h"
#include "qb_worldcharacter.h"
#include "kismet/gameplaystatics.h"

Aability_force_gun::Aability_force_gun() {

}

void Aability_force_gun::ActivateAbility() {
	Super::ActivateAbility();

	TArray<UActorComponent*> actor_comps;
	actor_comps = GetComponentsByTag(USceneComponent::StaticClass(), FName(BULLET_SPAWN_TAG));
	if (actor_comps.Num()) {
		USceneComponent* sc = Cast<USceneComponent>(actor_comps[0]);
		FVector start = sc->GetComponentLocation();
		FVector end = start + sc->GetComponentRotation().Vector() * range;
		FHitResult hit;
		TArray<AActor*> ignored;
		ignored.Add(this);
		ignored.Add(owning_gladiator);
		FCollisionQueryParams query_params;
		query_params.AddIgnoredActors(ignored);
		GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Camera, query_params);
		if (hit.bBlockingHit) {
			ACharacter* hit_c = Cast<ACharacter>(hit.GetActor());
			if (IsValid(hit_c)) {
				//prints("Applying force on %s", *(hit_c->GetName()));
				FVector force_dir = (hit_c->GetActorLocation() - sc->GetComponentLocation());
				force_dir.Z = hit_c->GetActorLocation().Z;
				force_dir.Normalize();
				hit_c->LaunchCharacter(force_dir * force_applied, false, false);

				float dist = FVector::Distance(GetActorLocation(), hit.GetActor()->GetActorLocation());
				float reduc = (dist / 2000.f) * (reduc_factor / 100.f);
				float app_damage = damage - (damage * reduc);
				UGameplayStatics::ApplyDamage(hit_c, app_damage, owning_controller, owning_gladiator, UDamageType::StaticClass());
			}
		}	else {
			//print("Force gun did not find anything");
		}
	}
	//else dbg("Invalid force spawn tag");
	OnUse();
	DeactivateAbility();
}

void Aability_force_gun::DeactivateAbility() {
	Super::DeactivateAbility();

}