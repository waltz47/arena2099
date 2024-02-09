// Fill out your copyright notice in the Description page of Project Settings.


#include "gladiators/boss_wizard.h"
#include "abilities/ability.h"
#include "aicontroller.h"
#include "projectiles/projectile.h"
#include "kismet/gameplaystatics.h"
#include "abilities/ability_gun.h"
#include "defs.h"
#include "kismet/kismetsystemlibrary.h"
#include "gameframework/charactermovementcomponent.h"
#include "qbplayer.h"

// Sets default values
Aboss_wizard::Aboss_wizard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aboss_wizard::BeginPlay()
{
	Super::BeginPlay();
	//FTimerHandle handle_teleport;
	//GetWorld()->GetTimerManager().SetTimer(handle_teleport, this, &Aboss_wizard::TryTeleport, teleport_cooldown, true, teleport_cooldown);
	rng = FRandomStream(GetUniqueID());
}

// Called every frame
void Aboss_wizard::Tick(float dt)
{
	Super::Tick(dt);

	if (!IsValid(enemy) || !IsValid(aicontroller)) return;
	//prints("canmv fwd: %d %d", canMoveForward.Num(),spawnedMinions.Num());
	for (int i = 0; i < canMoveForward.Num(); i++) {
		if (spawnedMinions.Num() <= i) {
			//dbg("minions > bools");
			continue;
		}
		if (!IsValid(spawnedMinions[i])) {
			//dbg("invalid minion");
			continue;
		}
		spawnedMinions[i]->AddActorWorldOffset(spawnedMinions[i]->GetActorForwardVector() * 20.f);
		//print("Moving forward");

	}
	//worker = new FMyWorker(this, 0, spawnedMinions.Num()-1);

	missile_launcher->SetActorRotation((enemy->GetActorLocation() - missile_launcher->GetActorLocation()).Rotation());
	rifle->SetActorRotation((enemy->GetActorLocation() - rifle->GetActorLocation()).Rotation());
	if ((GetGameTimeSinceCreation() - last_ability_at) <= ability_spacing) {
		//worker->Thread->WaitForCompletion();
		//delete worker;
		return;
	}
	if (ability_pending) {
		//worker->Thread->WaitForCompletion();
		//delete worker;
		return;
	}

	if (rifle->IsAbilityOffCooldown() && wizardSpellTypes.Num()) {
		//aim rifle at player
		Cast<Aability_gun>(rifle)->projectileType = wizardSpellTypes[rng.RandRange(0, wizardSpellTypes.Num()-1)];
		
		//rifle->ActivateAbility();
		last_ability_at = GetGameTimeSinceCreation();
		On_SparkAbilityUsed();
		ability_pending = true;
		//worker->Thread->WaitForCompletion();
		//delete worker;
		return;
	}


	if (missile_launcher->IsAbilityOffCooldown() && rng.RandRange(0,1)) {
		//missile_launcher->ActivateAbility();
		last_ability_at = GetGameTimeSinceCreation();
		On_WaterballRainAbilityUsed();
		ability_pending = true;
		//worker->Thread->WaitForCompletion();
		//delete worker;
		return;
	}

	if (CanSpawnMinions() && rng.RandRange(0,1)) {
		SpawnMinions();
		//worker->Thread->WaitForCompletion();
		//delete worker;
		return;
	}

	if (ShieldParamsMet() && shield->IsAbilityOffCooldown() && rng.RandRange(0,1)) {
		//shield->ActivateAbility();
		last_ability_at = GetGameTimeSinceCreation();
		ability_pending = true;
		On_ShieldAbilityUsed();
		//worker->Thread->WaitForCompletion();
		//delete worker;
		return;
	}

	if (IsValid(splitter) && splitter->IsAbilityOffCooldown() && (rng.FRandRange(0.f, 1.f) <= 0.57f)) {
		ability_pending = true;
		last_ability_at = GetGameTimeSinceCreation();
		On_SplitterUsed();
		//worker->Thread->WaitForCompletion();
		//delete worker;
		return;
	}
	//worker->Thread->WaitForCompletion();
	//delete worker;

}

void Aboss_wizard::AbiltyUseCalilback(AAbility* ability) {
	if (!IsValid(ability) || ability->IsAbilityOffCooldown() == false) {
			print("Invalid call to ability use callback.");
			return;
	}
	ability->ActivateAbility();
}

float Aboss_wizard::TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator)
{
	dmg = Super::TakeDamage(dmg, evt, ctrl, instigator);
	if ((phase == eWizardPhase::PHASE_I) && (hp / max_hp <= 0.5f)) {
		phase = eWizardPhase::PHASE_II;
	}
	return dmg;
}


bool Aboss_wizard::IsAnyAbilityActive() {
	bool any_act = false;
	any_act |= (false);

	return any_act;
}

bool Aboss_wizard::ShieldParamsMet() {
	return (phase == eWizardPhase::PHASE_II) && ((hp / max_hp) <= 0.4);
}


void Aboss_wizard::TryTeleport() {
	if (rifle->IsAbilityActive()) return;
	if (shield->IsAbilityActive()) return;
	if (missile_launcher->IsAbilityActive()) return;
	if (ability_pending) return;
	last_teleport_at = GetGameTimeSinceCreation();

	if (eqs_TeleportTo) {
		FEnvQueryRequest req(eqs_TeleportTo, this);
		req.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &Aboss_wizard::OnTeleportEQSOver);
	}
}
void Aboss_wizard::OnTeleportEQSOver(TSharedPtr<FEnvQueryResult> result) {
	if (result.IsValid() && IsValid(this) && IsValid(aicontroller) && result->IsSuccsessful()) {
		//prints("TeleportCalled");
		teleport_loc = result->GetItemAsLocation(0);
		TeleportEffectsOn();
		
		GetWorld()->GetTimerManager().SetTimer(th, this, &Aboss_wizard::WiTeleportTo, 1.f, false, -1);
		On_TeleportAbilityUsed();
		aicontroller->StopMovement();
	}
}

void Aboss_wizard::WiTeleportTo() {
	SetActorLocation(teleport_loc + FVector(0, 0, 100.f));
	TeleportEffectsOff();
}

bool Aboss_wizard::CanSpawnMinions() {
	return minionClass && ((GetGameTimeSinceCreation() - last_spawn_at) > minion_cooldown);
}

void Aboss_wizard::SpawnMinions() {
	last_spawn_at = GetGameTimeSinceCreation();
	if (eqs_MinionSpawnPointsEQS) {
		FEnvQueryRequest req(eqs_MinionSpawnPointsEQS, this);
		req.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &Aboss_wizard::OnMinionEQSEnd);
	}
}

void Aboss_wizard::OnMinionEQSEnd(TSharedPtr<FEnvQueryResult> result) {
	if (result.IsValid() && result->IsSuccsessful() && IsValid(this)) {
		TArray<FVector> locs;
		result->GetAllAsLocations(locs);
		for (int i = 0; i < FMath::Min(locs.Num(), num_spawns); i++) {
			TArray<AActor*> out;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), minionClass, out);
			if (out.Num() >= maxAllowedMinions) break; //dont spawn if minion limit reached
			FActorSpawnParameters spawn_params;
			spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AActor* spawned = GetWorld()->SpawnActor<AActor>(minionClass, locs[i] + FVector(0,0,100.f), FRotator::ZeroRotator, spawn_params);
			Aqb_worldcharacter* spawned_c = Cast<Aqb_worldcharacter>(spawned);
			if (IsValid(spawned_c)) {
				spawned_c->enemy = enemy;
				spawned_c->max_hp = max_hp / 3.f;
				spawned_c->hp = max_hp / 3.f;
			}
			On_MinionSpawned(locs[i] + FVector(0, 0, 100.f));

			spawnedMinions.Add(spawned);
			canMoveForward.Add(false);
		}
	}
}

bool Aboss_wizard::CanTeleport() {
	return (GetGameTimeSinceCreation() - last_teleport_at) > teleport_cooldown;
}