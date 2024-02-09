// Fill out your copyright notice in the Description page of Project Settings.


#include "gladiators/boss_mech.h"
#include "abilities/ability.h"
#include "aicontroller.h"
#include "projectiles/projectile.h"
#include "kismet/gameplaystatics.h"
#include "defs.h"
#include "kismet/kismetsystemlibrary.h"
#include "gameframework/charactermovementcomponent.h"
#include "qbplayer.h"

// Sets default values
Aboss_mech::Aboss_mech()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aboss_mech::BeginPlay()
{
  Super::BeginPlay();
  
}

// Called every frame
void Aboss_mech::Tick(float dt)
{
  Super::Tick(dt);
  if (!IsValid(aicontroller)) {
    dbg("Invalid AI Controller. Destorying actor");
    Destroy();
    return;
  }
  if (!IsValid(enemy)) {
    //dbg("No enemy");
    return;
  }
  /************************* STATUS EFFECTS *************************/

  bool canDash = CanDash();
  bool canUseMinigun = IsValid(minigun) && minigun->IsAbilityOffCooldown(); //no need to stop firing, increase reload time
  bool canUseMissileBarrage = IsValid(missile_launcher) && missile_launcher->IsAbilityOffCooldown();
  bool canUseLaser = IsValid(laser) && laser->IsAbilityOffCooldown();
  bool aboutToReceiveDamage = AboutToReceiveDamage();
  bool onLowHealth = (hp / max_hp <= 0.3f); //40% limit
  bool canUseForceGun = (IsValid(force_gun) && force_gun->IsAbilityOffCooldown());
  bool canSmokeEvade = IsValid(smoke_evade) && smoke_evade->IsAbilityOffCooldown();
  bool MinigunActive = (IsValid(minigun) && (minigun->IsAbilityActive() == true));
  bool LaserActive = (IsValid(laser) && (laser->IsAbilityActive() == true));

  if (is_dashing) {
    FVector t_dir = (dashing_to - GetActorLocation());
    t_dir.Normalize();
    AddActorWorldOffset(t_dir * dash_speed * dt);
    //prints("Dash movement");
    if (FVector::Distance(dashing_to, GetActorLocation()) <= 100.f) {
      //prints("Stopping Dash");
      EndDash();
    }
  }

  if (canDash && aboutToReceiveDamage) {
    TArray<FVector> dashpoints = GetOptimalDashPoint();
    if (dashpoints.Num() > 0) {
      int32 dp = rng.RandRange(0, dashpoints.Num() - 1);
      dashing_to = dashpoints[dp];
      //prints("Dashing");
      StartDash();
    }
  }


  if (CanSpawnMinions() && rng.RandRange(0, 1)) {
    SpawnMinions();
    //worker->Thread->WaitForCompletion();
    //delete worker;
    return;
  }


  if (canSmokeEvade && onLowHealth) {

    if (IsValid(minigun) && minigun->IsAbilityActive()) minigun->DeactivateAbility();
    if (IsValid(laser) && laser->IsAbilityActive()) laser->DeactivateAbility();
    if (IsValid(force_gun) && force_gun->IsAbilityActive()) force_gun->DeactivateAbility();
    if (IsValid(missile_launcher) && missile_launcher->IsAbilityActive()) missile_launcher->DeactivateAbility();

    smoke_evade->ActivateAbility();
    BP_OnSmokeEvade();
    GetCharacterMovement()->MaxWalkSpeed = 1200.f;
    return;
  }

  if (IsValid(smoke_evade) && smoke_evade->IsAbilityActive()) {
    //print("smoke active");
    if (!is_dashing && !canDash) {
      last_dash_at = -inf;
    }
    hp += smoke_evade_heal;
    if (hp > max_hp) {
      hp = max_hp;
    }
    return; //don't do stuff while using smoke evasion
  }
  else {
    GetCharacterMovement()->MaxWalkSpeed = 600.f; //normal movespeed
  }


  if (canUseForceGun && (FVector::Distance(force_gun->GetActorLocation(), enemy->GetActorLocation()) <= 800)) {
    FRotator torot = (enemy->GetActorLocation() - force_gun->GetActorLocation()).Rotation();
    force_gun->SetActorRotation(FMath::RInterpConstantTo(force_gun->GetActorRotation(), torot, dt, 80.f));
    //print("Rotating force gun");
    if (force_gun->GetActorRotation().Equals(torot)) {
      force_gun->ActivateAbility();
    }
  }
  else if (IsValid(force_gun) && force_gun->IsAbilityActive() == false) {
    //reset position
    FRotator torot = FRotator::ZeroRotator;
    force_gun->SetActorRotation(FMath::RInterpConstantTo(force_gun->GetActorRotation(), GetActorForwardVector().Rotation(), dt, COMPONENT_ROT_SPEED * 20.f));
  }

  if (IsValid(minigun) && minigun->IsAbilityActive()) {
    FVector enemy_velo = enemy->GetVelocity();
    enemy_velo.Normalize();
    FVector projected_enemy_pos = enemy->GetActorLocation() + enemy_velo* FMath::FRandRange(50.f, 150.f);
    FRotator torot = (projected_enemy_pos - minigun->GetActorLocation()).Rotation();
    minigun->SetActorRotation(FMath::RInterpTo(minigun->GetActorRotation(), torot, dt, COMPONENT_ROT_SPEED));
  }
  if (IsValid(laser) && laser->IsAbilityActive()) {
    FRotator torot = (enemy->GetActorLocation() - laser->GetActorLocation()).Rotation();
    laser->SetActorRotation(FMath::RInterpTo(laser->GetActorRotation(), torot, dt, 1.5f));
  }

  if ((GetGameTimeSinceCreation() - last_ability_at) <= time_bw_abilities) {
    return;
  }

  if (canUseMinigun && !LaserActive && !MinigunActive) {
    minigun->ActivateAbility();
    last_ability_at = GetGameTimeSinceCreation();
    //print("Activating Minigun");
    return;
  }

  if (canUseMissileBarrage && !MinigunActive && !LaserActive) {
    missile_launcher->ActivateAbility();
    last_ability_at = GetGameTimeSinceCreation();
    return;
  }
  if (canUseLaser && !MinigunActive) {
    laser->ActivateAbility();
    last_ability_at = GetGameTimeSinceCreation();
    return;
  }


}

bool Aboss_mech::AboutToReceiveDamage()
{
  if (last_dmg_flag) {
    last_dmg_flag = false;
    return true;

  }
  TArray<AActor*> overlapping_projectiles;
  TArray<TEnumAsByte<EObjectTypeQuery>> obj_types;
  obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
  obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
  obj_types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Camera));
  TArray<AActor*> ignored;
  UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 300.f, obj_types, Aprojectile::StaticClass(), ignored, overlapping_projectiles);
  for (AActor* pr : overlapping_projectiles) {
    if (!IsValid(pr)) continue;
    if (Cast<Aprojectile>(pr)->shot_by != this) {
      return true;
    }
  }
  return false;
}

bool Aboss_mech::CanSpawnMinions() {
  return minionClass && ((GetGameTimeSinceCreation() - last_spawn_at) > minion_cooldown);
}

void Aboss_mech::SpawnMinions() {
  last_spawn_at = GetGameTimeSinceCreation();
  if (eqs_MinionSpawnPointsEQS) {
    FEnvQueryRequest req(eqs_MinionSpawnPointsEQS, this);
    req.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &Aboss_mech::OnMinionEQSEnd);
  }
}

/* 

Keep a min dist of 700-800 from the player
Shoot whenever possible
Dash whenever possible
Use missiles when player isn't moving
Use laser when reloading

*/

float Aboss_mech::TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator)
{
  dmg = Super::TakeDamage(dmg, evt, ctrl, instigator);
  last_dmg_flag = true;
  
  return dmg;
}

void Aboss_mech::OnMinionEQSEnd(TSharedPtr<FEnvQueryResult> result) {
  if (result.IsValid() && result->IsSuccsessful() && IsValid(this)) {
    TArray<FVector> locs;
    result->GetAllAsLocations(locs);
    for (int i = 0; i < FMath::Min(locs.Num(), num_spawns); i++) {
      TArray<AActor*> out;
      UGameplayStatics::GetAllActorsOfClass(GetWorld(), minionClass, out);
      if (out.Num() >= maxAllowedMinions) break; //dont spawn if minion limit reached
      FActorSpawnParameters spawn_params;
      spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
      AActor* spawned = GetWorld()->SpawnActor<AActor>(minionClass, locs[i] + FVector(0, 0, 100.f), FRotator::ZeroRotator, spawn_params);
      Aqb_worldcharacter* spawned_c = Cast<Aqb_worldcharacter>(spawned);
      if (IsValid(spawned_c)) {
        spawned_c->enemy = enemy;
        //spawned_c->max_hp = max_hp / 3.f;
        //spawned_c->hp = max_hp / 3.f;
      }
      On_MinionSpawned(locs[i] + FVector(0, 0, 100.f));
    }
  }
}