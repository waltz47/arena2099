// Fill out your copyright notice in the Description page of Project Settings.


#include "gladiators/boss_skeleton.h"
#include "abilities/ability.h"
#include "aicontroller.h"
#include "projectiles/projectile.h"
#include "kismet/gameplaystatics.h"
#include "defs.h"
#include "kismet/kismetsystemlibrary.h"
#include "gameframework/charactermovementcomponent.h"
#include "qbplayer.h"

Aboss_skeleton::Aboss_skeleton()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aboss_skeleton::BeginPlay()
{
  Super::BeginPlay();

  attack_range = FMath::RandRange(800.f, 2600.f);
  playerenemy = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  maxZ = FMath::FRandRange(300.f, 500.f);
  diffZ = FMath::FRandRange(-100.f, 100.f);
}

// Called every frame
void Aboss_skeleton::Tick(float dt)
{
  Super::Tick(dt);
  if (IsValid(playerenemy) && IsValid(aicontroller)) {
    

    maxZ = playerenemy->GetActorLocation().Z + 300.f + diffZ;
    if (playerenemy->GetActorLocation().Z > GetMeshZ()) {
      return;
    }

    if ((FVector::Dist(GetActorLocation(), playerenemy->GetActorLocation()) <= attack_range) && CanAttack()) {
      last_attack_at = GetGameTimeSinceCreation();
      BP_OnAttack();
    }
    else  { 
      aicontroller->MoveToLocation(playerenemy->GetActorLocation(), attack_range , true, true, true, true);
    }
  }

}

bool Aboss_skeleton::CanAttack() {
  return (GetGameTimeSinceCreation() - last_attack_at) > attack_rate;
}