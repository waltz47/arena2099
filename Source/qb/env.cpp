// Fill out your copyright notice in the Description page of Project Settings.


#include "env.h"
#include "kismet/gameplaystatics.h"
#include "kismet/kismetsystemlibrary.h"
#include "gameframework/actor.h"
#include "aicontroller.h"

bool Uenv::IsGroundAt(UWorld* world, FVector loc) {
	if (!IsValid(world)) {
		return false;
	}
	FVector end = loc;
	end.Z = -10000.f;
	FHitResult hit;
	world->LineTraceSingleByChannel(hit, loc, end, ECollisionChannel::ECC_WorldStatic);
	return hit.bBlockingHit;
}

FVector Uenv::GetGroundAt(UWorld* world, FVector loc) {
	if (!IsValid(world)) {
		return FVector::ZeroVector;
	}
	FVector end = loc;
	end.Z = -10000.f;
	FHitResult hit;
	world->LineTraceSingleByChannel(hit, loc, end, ECollisionChannel::ECC_WorldStatic);
	if (hit.bBlockingHit == false) return FVector::ZeroVector;
	return hit.Location;
}

