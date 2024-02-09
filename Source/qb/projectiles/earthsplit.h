// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "earthsplit.generated.h"

UCLASS()
class QB_API Aearthsplit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aearthsplit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float damage = 10.f;
	UPROPERTY() AActor* player = nullptr; // so dont have to search for player on every spawn


};
