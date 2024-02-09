// Fill out your copyright notice in the Description page of Project Settings.


#include "projectiles/missile.h"
#include "kismet/gameplaystatics.h"
#include "kismet/kismetsystemlibrary.h"
#include "defs.h"
void Amissile::BeginPlay() {
	Super::BeginPlay();

	epoch = GetActorLocation() + FVector(FMath::RandRange(-400.f, 400.f), FMath::RandRange(-400.f, 400.f), max_altitude);

}


void Amissile::Tick(float dt) {
	Super::Tick(dt);
	if (!is_active) return;
	if (!IsValid(enemy)) {
		Destroy();
		return;
	}
	if (moving_up) {
		FVector towards = epoch - GetActorLocation();
		SetActorRotation(towards.Rotation());
		towards.Normalize();
		
		AddActorWorldOffset(towards * speed * dt);
		if (GetActorLocation().Z >= max_altitude) {
			moving_up = 0;
			last_known_pos = enemy->GetActorLocation();
		}
	}
	else {
		if (towards_down == FVector::ZeroVector) {
			towards_down = enemy->GetActorLocation() - GetActorLocation();
			towards_down.Normalize();
		}
		SetActorRotation(towards_down.Rotation());
		
		AddActorWorldOffset(towards_down*speed * dt);
		
		TArray<AActor*> overlapping;
		GetOverlappingActors(overlapping);
		if (overlapping.Num()) {
			//prints("overlapping %d", overlapping.Num());
			
			TArray<AActor*> overlapping_projectiles;
			TArray<TEnumAsByte<EObjectTypeQuery>> obj_types;
			obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
			obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Camera));
			obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
			TArray<AActor*> ignored;
			UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), radius, obj_types, AActor::StaticClass(), ignored, overlapping_projectiles);

			for (AActor* actor : overlapping_projectiles) {
				UGameplayStatics::ApplyDamage(actor, projectileDamage, shot_by_controller, shot_by, UDamageType::StaticClass());
				dbg("Missile damage actor");
			}

			OnExplode();
			is_active = false;
			Destroy();
		}
	}

}

void Amissile::Explode() {
	Super::Explode();
	TArray<AActor*> overlapping;
	GetOverlappingActors(overlapping);
	if (overlapping.Num()) {
		//prints("overlapping %d", overlapping.Num());

		TArray<AActor*> overlapping_projectiles;
		TArray<TEnumAsByte<EObjectTypeQuery>> obj_types;
		obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Camera));
		obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> ignored;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), radius, obj_types, AActor::StaticClass(), ignored, overlapping_projectiles);

		for (AActor* actor : overlapping_projectiles) {
			UGameplayStatics::ApplyDamage(actor, projectileDamage, shot_by_controller, shot_by, UDamageType::StaticClass());
			//dbg("Missile damage actor");
		}
	}
	OnExplode();
	is_active = false;
	Destroy();
}
