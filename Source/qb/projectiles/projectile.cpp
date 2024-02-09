// Fill out your copyright notice in the Description page of Project Settings.


#include "projectiles/projectile.h"

// Sets default values
Aprojectile::Aprojectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	SetRootComponent(sceneRoot);

	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	projectileMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void Aprojectile::BeginPlay()
{
	Super::BeginPlay();
	
	float ex = FMath::FRandRange(-0.03, 0.07f);
	projectileDamage += (projectileDamage * ex);
}

// Called every frame
void Aprojectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float Aprojectile::TakeDamage(float dmg, const FDamageEvent& evt, AController* ctrl, AActor* instigator) {
	dmg = Super::TakeDamage(dmg, evt, ctrl, instigator);
	if (dmg) {
		dmginstances++;
		if (dmginstances == dmg_instances_to_destroy) {
			Explode();
		}
	}
	return dmg;
}

