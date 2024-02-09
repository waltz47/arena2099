//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "qbCharacter.h"
//#include "defs.h"
//#include "env.h"
//#include "Serialization/BufferArchive.h"
//#include "projectiles/projectile.h"
//#include "kismet/kismetsystemlibrary.h"
//#include "projectiles/missile.h"
//
//// Sets default values
//AqbCharacter::AqbCharacter()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//	attributecomp = CreateDefaultSubobject<UAttributeComponent>(TEXT("attribute component"));
//
//}
//
//// Called when the game starts or when spawned
//void AqbCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
//	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
//	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
//	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
//	TArray<AActor*> ignored;
//	ignored.Add(this);
//	TArray<AActor*> overlapping;
//
//	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 1000.f, objectTypesArray,
//		AqbCharacter::StaticClass(), ignored, overlapping);
//
//	for (AActor* actor : overlapping) {
//		if (!IsValid(actor)) continue;
//		if (!IsValid(Cast<AqbCharacter>(actor))) continue;
//		if (actor == this) continue;
//		enemy = Cast<AqbCharacter>(actor);
//		break;
//	}
//	if (!IsValid(enemy)) {
//		dbg("Enemy not found");
//		//Destroy();
//	}
//	rng = FRandomStream(FDateTime::Now().GetMillisecond());
//	prev_state = GetCurrentState();
//}
//
//// Called every frame
//void AqbCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//
//	//qmap eval
//	if ((GetGameTimeSinceCreation() - last_eval_at) >= evaluate_timer) {
//		Evaluate();
//	}
//
//
//
//
//	if (isDashing) {
//		//character is dashing
//		FVector steploc = FMath::VInterpTo(GetActorLocation(), dashloc, DeltaTime, 10.f);
//		SetActorLocation(steploc);
//		if (FVector::Distance(GetActorLocation(), dashloc) <= 100.f) {
//			StopDash();
//		}
//	}
//
//
//}
//
//// Called to bind functionality to input
//void AqbCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}
//
//float AqbCharacter::TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator)
//{
//	dmg = Super::TakeDamage(dmg, evt, ctrl, instigator);
//	dmg = attributecomp->GetDamageTaken(dmg);
//	attributecomp->health -= dmg;
//
//	if (attributecomp->health <= 0.f) {
//		OnDeath();
//	}
//	damage_taken += dmg;
//	if (IsValid(enemy)) {
//		enemy->damage_done += dmg;
//	}
//	//prints("%s took %f damage", *(GetName()), dmg);
//	return dmg;
//}
//
//void AqbCharacter::OnDeath() {
//	attributecomp->health = 100.f;
//}
//
//AgentState AqbCharacter::GetCurrentState() {
//	AgentState s;
//	return s;
//}
//
//pair<AgentAction, float> AqbCharacter::GetAction_Max(AgentState state) {
//	AgentAction action;
//	float qvalue = -inf;
//
//	for (const auto& it : qmap) {
//		if (it.first.first == state && it.second > qvalue) {
//			qvalue = it.second;
//			action = it.first.second;
//		}
//	}
//	if (qvalue == -inf) {
//		qvalue = 0.f;
//		action = GetRandomAction(state);
//	}
//	return std::make_pair(action, qvalue);
//}
//
//AgentAction AqbCharacter::GetAction(AgentState state) {
//
//	tks++;
//	if (tks >= decay_ticks) {
//		tks = 0;
//		Decay();
//	}
//
//	float prob = rng.FRandRange(0.f, 1.f);
//	AgentAction action;
//	float scr = -inf;
//	if (prob >= eps) {
//		std::tie(action, scr) = GetAction_Max(state);
//	}
//	if (prob < eps || scr == -inf) {
//		action = GetRandomAction(state);
//	}
//	return action;
//}
//
//AgentAction AqbCharacter::GetRandomAction(AgentState state) {
//	AgentAction action;
//	return action;
//}
//
//
//void AqbCharacter::Evaluate() {
//	AgentState curr_state = GetCurrentState();
//	float reward = GetReward();
//
//	float prev_value = qmap[{prev_state, action_taken}];
//	qmap[{prev_state, action_taken}] = prev_value + lr * (reward + discount * GetAction_Max(curr_state).second - prev_value);
//
//	action_taken = GetAction(curr_state);
//	prev_state = curr_state;
//
//	ActuateAction(action_taken);
//
//	if (lr == min_lr) {
//		//PrintQMap();
//		//Destroy();
//	}
//
//	ClearStats();
//
//}
//
//void AqbCharacter::PrintQMap() {
//	float tstamp = GetGameTimeSinceCreation();
//	FBufferArchive archive;
//
//	//for (const auto& it : qmap) {
//	//	const AgentState st = it.first.first;
//	//	const AgentAction ac = it.first.second;
//
//	//	archive << (int32)st.dist_to_enemy;
//	//	archive << (int32)st.cooldowns;
//	//	archive << (int32)st.available_dash_positions;
//
//	//	archive << (int32)ac.move_to_pos;
//	//	archive << (int32)ac.use_ability;
//	//	archive << (float)it.second;
//	//}
//	TArray<uint8> Data;
//	Data.Append(archive.GetData(), archive.Num());
//	FString pt = (FPaths::GameSourceDir() + FString("ql.txt"));
//	FFileHelper::SaveArrayToFile(Data, *pt);
//	//dbg("written to txt file");
//	//dbg_str("qmap size is: %d", qmap.size());
//
//	//ReadQMap();
//
//	//dbg_str("Time for w + r: %f", GetGameTimeSinceCreation() - tstamp);
//}
//
//void AqbCharacter::ReadQMap() {
//	TArray<uint8>  data;
//	FString pt = (FPaths::GameSourceDir() + FString("ql.txt"));
//	FFileHelper::LoadFileToArray(data, *pt);
//	FMemoryReader memrd(data, true);
//	qmap.clear();
//	while (memrd.TotalSize() - memrd.Tell() > 0) {
//		AgentState st;
//		AgentAction ac;
//		//float val;
//
//		/*memrd << (int32)st.dist_to_enemy;
//		memrd << (int32)st.cooldowns;
//		memrd << (int32)st.available_dash_positions;
//
//		memrd << (int32)ac.move_to_pos;
//		memrd << (int32)ac.use_ability;
//		memrd << val;*/
//		//qmap[{st, ac}] = val;
//	}
//	dbg_str("qmap size after reading: %d", qmap.size());
//}
//
//float AqbCharacter::GetReward() {
//	float ret = 0;
//	ret += damage_done * w_damage_done;
//	ret += damage_taken * w_damage_taken;
//	ret += w_using_ability_on_cd * (int)invalid_usage;
//
//	if (damage_taken == 0.f && prev_state.targeted_by_enemy) {
//		ret += w_projectile_dodged;
//	}
//	return ret;
//}
//
//void AqbCharacter::Decay() {
//	dbg_str("qmap size: %d", qmap.size());
//	lr = FMath::Max(min_lr, lr * lr_decay);
//	eps = FMath::Max(min_eps, eps * eps_decay);
//}
//
//
//void AqbCharacter::ClearStats() {
//	damage_done = 0.f;
//	damage_taken = 0.f;
//	invalid_usage = 0;
//
//	last_eval_at = GetGameTimeSinceCreation();
//}
//
//void AqbCharacter::ActuateAction(AgentAction act) {
//
//}
//
////DASHING
//
//void AqbCharacter::Dash(FVector loc) {
//	if ((GetGameTimeSinceCreation() - last_dash_at) <= dashCooldown) {
//		invalid_usage = true;
//		return;
//	}
//	dashloc = loc;
//	isDashing = true;
//	last_dash_at = GetGameTimeSinceCreation();
//}
//
//void AqbCharacter::StopDash() {
//	isDashing = false;
//}
//
////LAUNCH MISSILE BARRAGE
//
//void AqbCharacter::LaunchMissileBarrage() {
//	if (!missileClass) {
//		dbg("Invalid missile class");
//		return;
//	}
//	if (((GetGameTimeSinceCreation() - last_barrage_at) <= barrageCooldown)) {
//		invalid_usage = true;
//		return;
//	}
//	last_barrage_at = GetGameTimeSinceCreation();
//
//	for (int32 i = 0; i < numMissiles; i++) {
//		Aprojectile* proj;
//		FActorSpawnParameters spawn_params;
//		spawn_params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//		proj = GetWorld()->SpawnActor<Aprojectile>(missileClass, GetActorLocation() + FVector(0, 0, 100.f), GetActorRotation(), spawn_params);
//		proj->shot_by = this;
//		proj->shot_by_controller = GetController();
//		Cast<Amissile>(proj)->enemy = enemy;
//		proj->is_active = true;
//		dbg("Launched Missile");
//	}
//}