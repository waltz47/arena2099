// Fill out your copyright notice in the Description page of Project Settings.


#include "projectiles/earthsplit.h"
#include "kismet/gameplaystatics.h"

// Sets default values
Aearthsplit::Aearthsplit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aearthsplit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aearthsplit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TArray<AActor*> overlapping;
	GetOverlappingActors(overlapping, AActor::StaticClass());

	if (overlapping.Num()) {
		//prints("overlapping %d", overlapping.Num());
		for (int i = 0; i < overlapping.Num(); i++) {
			if (overlapping[i] != player) continue;
			float dmg = UGameplayStatics::ApplyDamage(overlapping[i], damage, nullptr, nullptr, UDamageType::StaticClass());
		}

		
	}

}

