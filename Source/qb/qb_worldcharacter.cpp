// Fill out your copyright notice in the Description page of Project Settings.


#include "qb_worldcharacter.h"
#include "aicontroller.h"
#include "kismet/gameplaystatics.h"
#include "qbplayer.h"
#include "env.h"

// Sets default values
Aqb_worldcharacter::Aqb_worldcharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aqb_worldcharacter::BeginPlay()
{
	Super::BeginPlay();
	FindAbilityWithTag();
	aicontroller = Cast<AAIController>(GetController());
	//move_to_loc = GetActorLocation();

	if (IsValid(aicontroller)) {
		GetWorld()->GetTimerManager().SetTimer(move_to_eqs_handle, this, &Aqb_worldcharacter::MoveToEQS, MoveToEqsTimer, true, MoveToEqsTimer);
	}
	rng = FRandomStream(GetUniqueID());
}

// Called every frame
void Aqb_worldcharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(enemy)) {
		FRotator torot = (enemy->GetActorLocation() - GetActorLocation()).Rotation();
		torot.Pitch = 0.f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), torot, DeltaTime, 5.f));
	}
}

// Called to bind functionality to input
void Aqb_worldcharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float Aqb_worldcharacter::TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator)
{
	dmg = Super::TakeDamage(dmg, evt, ctrl, instigator);

	dmg = (dmg * dmg / (dmg + armor));
	hp -= dmg;
	dbg_str("took %f damage", dmg);
	if (!IsValid(enemy) && IsValid(instigator)) {
		enemy = Cast<Aqb_worldcharacter>(instigator);
	}
	if (hp < 0.f) {
		OnDeath();
	}
	DamageNumbers(dmg);

	return dmg;
}


void Aqb_worldcharacter::OnDeath() {
	BP_Death();
	Aqbplayer* player = Cast<Aqbplayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (IsValid(player) && player != this) {
		player->SpawnNextBoss(FName(gladiator_name));
	}
	Destroy();
}


void Aqb_worldcharacter::FindAbilityWithTag() {
	TArray<AActor*> attached;
	GetAttachedActors(attached);
	for (AActor* actor:attached) {
		AAbility* ab = Cast<AAbility>(actor);
		if (!IsValid(ab)) continue;
		ab->owning_gladiator = this;
		ab->owning_controller = GetController();
		if (ab->ActorHasTag(FName(MB_TAG))) {
			missile_launcher = ab;
			//print("Added Missile Launcher");
		}
		if (ab->ActorHasTag(FName(MINIGUN_TAG))) {
			//print("Added Minigun");
			minigun = ab;
		}
		if (ab->ActorHasTag(FName(LASER_TAG))) {
			//print("Added Laser");
			laser = ab;
		}
		if (ab->ActorHasTag(FName(RIFLE_TAG))) {
			//for player
			//print("Added rifle");
			rifle = ab;
		}
		if (ab->ActorHasTag(FName(SMOKE_EVADE_TAG))) {
			//for player
			//print("Added smoke evasion");
			smoke_evade = ab;
		}
		if (ab->ActorHasTag(FName(FORCE_GUN_TAG))) {
			//for player
			//print("Added force gun");
			force_gun = ab;
		}
		if (ab->ActorHasTag(FName(SHIELD_TAG))) {
			//for player
			//print("Added shield");
			shield = ab;
		}
		if (ab->ActorHasTag(FName(EARTH_SPLITTER_TAG))) {
			//for player
			//print("Added splitter");
			splitter = ab;
		}
	}
}

//DASH

bool Aqb_worldcharacter::CanDash() {
	return !is_dashing && ((GetGameTimeSinceCreation() - last_dash_at) > dash_cooldown);
}
void Aqb_worldcharacter::StartDash() {
	last_dash_at = GetGameTimeSinceCreation();
	is_dashing = true;
	if (IsValid(aicontroller)) {
		aicontroller->StopMovement();
	}
	BP_Dash();
}
void Aqb_worldcharacter::EndDash() {
	is_dashing = false;
	BP_StopDash();
}	

void Aqb_worldcharacter::OnSeeEnemy(Aqb_worldcharacter* character) {
	if (IsValid(character)) {
		enemy = character;
	}
}

void Aqb_worldcharacter::OnMoveToEQSCompleted(TSharedPtr<FEnvQueryResult> result) {
	if (result.IsValid() && result->IsSuccsessful()) {
		move_to_loc = result->GetItemAsLocation(0);
		//print("move to eqs succeeded");
		if (IsValid(aicontroller)) {
			aicontroller->MoveToLocation(move_to_loc, 100.f, true);
		}
	}
	else {
		move_to_loc = FVector::ZeroVector;
		//print("move to eqs failed");
	}

	MoveToEqsRunning = false;
}

TArray<FVector> Aqb_worldcharacter::GetOptimalDashPoint() {
	FVector left = GetActorLocation() + GetActorRightVector() * -1.f * dash_dist;
	FVector right = GetActorLocation() + GetActorRightVector() * dash_dist;
	FVector back = GetActorLocation() + GetActorForwardVector() * -1.f * dash_dist;
	FVector front = GetActorLocation() + GetActorForwardVector() * dash_dist;
	TArray<FVector> points;
	if (!IsValid(enemy)) return points;
	if (Uenv::IsGroundAt(GetWorld(),left) && FVector::Distance(left ,enemy->GetActorLocation()) >= 800.f) {
			points.Add(left);
	}
	if (Uenv::IsGroundAt(GetWorld(), right) && FVector::Distance(right ,enemy->GetActorLocation()) >= 800.f) {
			points.Add(right);
	}
	if (Uenv::IsGroundAt(GetWorld(), back) && FVector::Distance(back ,enemy->GetActorLocation()) >= 800.f) {
			points.Add(back);
	}
	if (Uenv::IsGroundAt(GetWorld(), front) && FVector::Distance(front ,enemy->GetActorLocation()) >= 800.f) {
			points.Add(front);
	}

	return points;
}

void Aqb_worldcharacter::MoveToEQS() {
	//because player inherits from this as well
	if (IsValid(aicontroller)) {
		if (eqs_MoveTo && MoveToEqsRunning == false && is_dashing == false) {
			FEnvQueryRequest req(eqs_MoveTo, this);
			req.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &Aqb_worldcharacter::OnMoveToEQSCompleted);
			MoveToEqsRunning = true;
		}
	}
}