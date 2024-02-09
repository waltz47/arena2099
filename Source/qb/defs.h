// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class QB_API defs
{
public:
	defs();
	~defs();
};

#define dbg(txt) UE_LOG(LogTemp, Warning, TEXT(txt))
#define dbg_str(txt,s) UE_LOG(LogTemp, Warning, TEXT(txt),s);

#define print(txt) if(GEngine){GEngine->AddOnScreenDebugMessage(FMath::Rand(), 1.f,FColor::Yellow, TEXT(txt));}
#define prints(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(FMath::Rand(), 1.f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}

/*#define print(txt) (void)0
#define prints(x, ...) (void)0*/


#define DASH_TAG "tag_dash"
#define MB_TAG "tag_missile_launcher"
#define MINIGUN_TAG "tag_minigun"
#define LASER_TAG "tag_laser"
#define RIFLE_TAG "tag_rifle"
#define SMOKE_EVADE_TAG "tag_smoke_evade"
#define FORCE_GUN_TAG "tag_force_gun"
#define SHIELD_TAG "tag_shield"
#define EARTH_SPLITTER_TAG "tag_splitter"

#define BULLET_SPAWN_TAG "tag_bullet_spawn"
#define LASER_SPAWN_TAG "tag_laser_spawn"

#define COMPONENT_ROT_SPEED 1.9f