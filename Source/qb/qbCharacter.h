//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include <map>
//#include <utility>
//#include <vector>
//#include "components/attributecomponent.h"
//#include "qbCharacter.generated.h"
//
//using std::map;
//using std::vector;
//using std::pair;
//
//#define inf 99999999
//#define max_num_actions 20
//
//struct AgentState {
//public:
//	int32 dist_to_enemy;
//	int32 cooldowns;
//	int32 available_dash_positions;
//	bool targeted_by_enemy;
//
//	AgentState() {
//		dist_to_enemy = 0;
//		cooldowns = 0;
//		available_dash_positions = 0;
//		targeted_by_enemy = false;
//	}
//
//	friend bool operator==(const AgentState& x, const AgentState& y) {
//		return std::tie(x.dist_to_enemy, x.cooldowns, x.available_dash_positions, x.targeted_by_enemy) ==
//			std::tie(y.dist_to_enemy, y.cooldowns, y.available_dash_positions,y.targeted_by_enemy);
//	}
//	friend bool operator<(const AgentState& x, const AgentState& y) {
//		return std::tie(x.dist_to_enemy, x.cooldowns, x.available_dash_positions, x.targeted_by_enemy) <
//			std::tie(y.dist_to_enemy, y.cooldowns, y.available_dash_positions, y.targeted_by_enemy);
//	}
//};
//
//struct AgentAction {
//public:
//	int32 move_to_pos;
//	int32 use_ability;
//
//	AgentAction() {
//		move_to_pos = 0;
//		use_ability = 0;
//	}
//
//	friend bool operator==(const AgentAction& x, const AgentAction& y) {
//		return std::tie(x.move_to_pos, x.use_ability) == std::tie(y.move_to_pos, y.use_ability);
//	}
//
//	friend bool operator<(const AgentAction& x, const AgentAction& y) {
//		return std::tie(x.move_to_pos, x.use_ability) < std::tie(y.move_to_pos, y.use_ability);
//	}
//};
//
//UCLASS()
//class QB_API AqbCharacter : public ACharacter
//{
//	GENERATED_BODY()
//
//public:
//	// Sets default values for this character's properties
//	AqbCharacter();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
//
//	UPROPERTY(VisibleAnywhere)
//		UAttributeComponent* attributecomp = nullptr;
//
//	virtual float TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator);
//
//	virtual void OnDeath();
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//		AqbCharacter* enemy = nullptr;
//
//
//	/******************************QL VARIABLES AND FUNCTIONS********************************/
//
//	//rng
//	FRandomStream rng;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		bool debug = 0;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		bool training = 1;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		float lr = 1.0;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		float min_lr = 0.05;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		float lr_decay = 0.99;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		float eps = 1.0;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		float min_eps = 0.01;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		float eps_decay = 0.97;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		float discount = 0.9;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QL")
//		int32 decay_ticks = 5000;
//
//	int32 tks = 0;
//
//	/*****************************REWARD WEIGHTS*******************************/
//	/**/
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weights")
//		float w_damage_done = 2.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weights")
//		float w_damage_taken = -1.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weights")
//		float w_using_ability_on_cd = -30.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weights")
//		float w_projectile_dodged = 10.f;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weights")
//		float evaluate_timer = 0.5f;
//
//
//
//	map<pair<AgentState, AgentAction>, float> qmap;
//	AgentState prev_state;
//	AgentAction action_taken;
//
//	virtual AgentState GetCurrentState();
//	virtual pair<AgentAction, float> GetAction_Max(AgentState state);
//	virtual AgentAction GetAction(AgentState state);
//	virtual AgentAction GetRandomAction(AgentState state);
//	void Evaluate();
//	virtual void ActuateAction(AgentAction act);
//	virtual float GetReward();
//	void ClearStats();
//	void Decay();
//
//	void PrintQMap();
//	void ReadQMap();
//
//
//
//	/*******************************STATS TO CALCULATE REWARDS**********************/
//
//	float damage_done;
//	float damage_taken;
//	bool invalid_usage;
//	float last_eval_at = -inf;
//
//
//	/*******************************ABILITIES**************************************/
//
//	TArray<class AAbility*> abilities;
//
//	//DASH
//	TArray<FVector> dash_positions;
//	TArray<bool> dashPosValid;
//	FVector dashloc;
//	bool isDashing = false;
//	virtual void Dash(FVector l);
//	virtual void StopDash();
//
//	UPROPERTY(EditAnywhere, Category = "Abilities")
//		float dashDistance = 700.f;
//
//	UPROPERTY(EditAnywhere, Category = "Abilities")
//		float dashCooldown = 2.f;
//
//	float last_dash_at = -inf;
//
//	//MISSILE BARRAGE
//
//	void LaunchMissileBarrage();
//
//	UPROPERTY(EditAnywhere, Category = "Abilities")
//		TSubclassOf<class Aprojectile> missileClass;
//
//	UPROPERTY(EditAnywhere, Category = "Abilities")
//		int32 numMissiles = 3;
//
//	UPROPERTY(EditAnywhere, Category = "Abilities")
//		float barrageCooldown = 3.f;
//
//	float last_barrage_at = -inf;
//
//	
//	//LASER
//
//	//virtual void LaserOn();
//	//virtual void LaserOff();
//	//
//	//UFUNCTION(BlueprintImplementableEvent)
//	//	void OnLaserOn();
//
//	//UFUNCTION(BlueprintImplementableEvent)
//	//	void OnLaserOff();
//	
//
//
//
//
//};
//
