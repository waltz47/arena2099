//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "gladiators/gladiator_cube.h"
//#include "abilities/ability_gun.h"
//#include "defs.h"
//#include "env.h"
//#include "projectiles/projectile.h"
//#include "abilities/ability.h"
//#include "abilities/ability_sword.h"
//#include "kismet/kismetsystemlibrary.h"
//
//Agladiator_cube::Agladiator_cube() {
//
//}
//
//void Agladiator_cube::BeginPlay() {
//	Super::BeginPlay();
//	if (!IsValid(enemy)) {
//		dbg("Destrying gladiator because enemy missing");
//		Destroy();
//	}
//	TArray<AActor*> ab_actors;
//	GetAttachedActors(ab_actors);
//	for (auto ab : ab_actors) {
//		if (Cast<AAbility>(ab)) {
//			abilities.Add(Cast<AAbility>(ab));
//		}
//	}
//	dbg_str("Number of Abilities: %d", abilities.Num());
///*	FString ActorName = GetName();
//	int32 UniqueNumber = FCString::Atoi(*ActorName) * GetUniqueID()*/;
//	rng = FRandomStream(GetUniqueID());
//}
//
//AgentState Agladiator_cube::GetCurrentState() {
//	AgentState s;
//	s.dist_to_enemy = 0;
//	s.cooldowns = 0;
//	s.available_dash_positions = 0;
//	s.targeted_by_enemy = false;
//	int cdmask = 0;
//	for (int i = 0; i < abilities.Num(); i++) {
//		if (abilities[i]->IsAbilityOffCooldown()) {
//			cdmask |= (1 << i);
//		}
//	}
//	if (((GetGameTimeSinceCreation() - last_dash_at) > dashCooldown))
//		cdmask |= (1 << abilities.Num());
//
//	if (((GetGameTimeSinceCreation() - last_barrage_at) > barrageCooldown))
//		cdmask |= (1 << (abilities.Num()+1));
//
//	s.cooldowns = cdmask;
//
//	//GET DASH POSITIONS
//	FVector left, right, towards, away;
//	left = GetActorLocation() + GetActorRightVector() * -1.f * dashDistance;
//	right = GetActorLocation() + GetActorRightVector() * 1.f * dashDistance;
//	towards = (enemy->GetActorLocation() - GetActorLocation());
//	towards.Normalize();
//	towards = GetActorLocation() + towards * dashDistance;
//	away = GetActorLocation() + towards * -1.f * dashDistance;
//
//	bool valid_l, valid_r, valid_t, valid_a;
//	valid_l = Uenv::IsGroundAt(GetWorld(), left);
//	valid_r = Uenv::IsGroundAt(GetWorld(), right);
//	valid_t = Uenv::IsGroundAt(GetWorld(), towards);
//	valid_a = Uenv::IsGroundAt(GetWorld(), away);
//
//	if (FVector::Distance(enemy->GetActorLocation(), GetActorLocation()) < (dashDistance - 50.f)) {
//		valid_t = false;
//	}
//	dash_positions.Empty();
//	dashPosValid.Empty();
//	dash_positions.Add(left);
//	dash_positions.Add(right);
//	dash_positions.Add(towards);
//	dash_positions.Add(away);
//
//	dashPosValid.Add(valid_l);
//	dashPosValid.Add(valid_r);
//	dashPosValid.Add(valid_t);
//	dashPosValid.Add(valid_a);
//
//	int32 dash_mask = valid_l;
//	if (valid_r) dash_mask |= 2;
//	if (valid_t) dash_mask |= 4;
//	if (valid_a) dash_mask |= 8;
//	s.available_dash_positions = dash_mask;
//
//
//	//check if projectile incoming
//	TArray<AActor*> overlapping_projectiles;
//	TArray<TEnumAsByte<EObjectTypeQuery>> obj_types;
//	obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
//	obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
//	obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Camera));
//	TArray<AActor*> ignored;
//	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 300.f, obj_types, Aprojectile::StaticClass(), ignored, overlapping_projectiles);
//	for (AActor* pr : overlapping_projectiles) {
//		if (!IsValid(pr)) continue;
//		if (Cast<Aprojectile>(pr)->shot_by != this) {
//			s.targeted_by_enemy = true;
//			if (debug) print("Targeted by enemy");
//			break;
//		}
//	}
//
//
//	return s;
//}
//
//AgentAction Agladiator_cube::GetRandomAction(AgentState state) {
//	if(debug) print("Getting random  action");
//	AgentAction action;
//	action.use_ability = (1 << rng.RandRange(0, abilities.Num()+2)); //last is doing nothing
//	action.move_to_pos = (1 << rng.RandRange(0, 3)); //last is no movement
//	return action;
//}
//
//void Agladiator_cube::ActuateAction(AgentAction act) {
//	Super::ActuateAction(act);
//
//	//for (auto it : abilities) {
//	//	it->DeactivateAbility();
//	//}
//
//	////attached abilities
//	//for (int i = 0; i < abilities.Num(); i++) {
//	//	if (act.use_ability & (1 << i)) {
//	//		if (abilities[i]->IsAbilityOffCooldown() == false) {
//	//			if(debug) dbg("Ability is on cooldown.");
//	//			invalid_usage = true;
//	//			return;
//	//		}
//	//		else {
//	//			abilities[i]->SetActorRotation((enemy->GetActorLocation() - abilities[i]->GetActorLocation()).Rotation());
//	//			abilities[i]->ActivateAbility();
//	//			if (debug) prints("Using Ability %d", i);
//	//			return;
//	//		}
//	//		
//	//		break;
//	//	}
//	//}
//
//	////character abilities
//
//	//if (act.use_ability == (1 << abilities.Num())) {
//	//	FVector dashposition;
//	//	for (int i = 0; i < 4; i++) {
//	//		if (act.move_to_pos & (1 << i)) {
//	//			dashposition = dash_positions[i];
//	//			if (dashPosValid[i] == false) {
//	//				if(debug) dbg("Teleport to invalid positions");
//	//				invalid_usage = true;
//	//				return;
//	//			}
//	//			break;
//	//		}
//	//	}
//	//	Dash(dashposition);
//	//	return;
//	//}
//	//else if (act.use_ability == (1 << (abilities.Num() + 1))) {
//	//	LaunchMissileBarrage();
//	//}
//	//if(debug) dbg("Not using any ability");
//
//
//}
//
////dash is coded in character
////only function call here
//
//void Agladiator_cube::Tick(float dt) {
//	Super::Tick(dt);
//	FRotator rot = (enemy->GetActorLocation() - GetActorLocation()).Rotation();
//	rot.Pitch = 0.f;
//	SetActorRotation(FMath::RInterpTo(GetActorRotation(), rot, dt, 20.f));
//
//	bool damageIncoming = false;
//	bool missilesOffCooldown = (GetGameTimeSinceCreation() - last_barrage_at > barrageCooldown);
//	bool dashOffCooldown = (GetGameTimeSinceCreation() - last_dash_at > dashCooldown);
//
//
//
//}