// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "env.generated.h"

/**
 * 
 */
UCLASS()
class QB_API Uenv : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool IsGroundAt(UWorld* world, FVector loc);
	static FVector GetGroundAt(UWorld* world, FVector loc);
	
};
