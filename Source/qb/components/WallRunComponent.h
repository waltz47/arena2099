// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallRunComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QB_API UWallRunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWallRunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
		void ResetAllVariables();

	/*Owning character*/
	UPROPERTY()
		class ACharacter* Player = nullptr;

	/*Character's Controller*/
	UPROPERTY()
		class AController* PlayerController = nullptr;

	UPROPERTY()
		class UCharacterMovementComponent* MovementComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		float TraceDistance = 500.f;

	UPROPERTY()
		FRotator DefaultControlRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		bool WallRunOver = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		bool AreWeWallRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		bool CanWallRun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		bool WallRunButtonPressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		float InterpResetSpeed = 5.f;

	/*-1 for left Hit 1 for Right and 0 for None*/
	UPROPERTY()
		int WallSelection = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		float CustomGravityScale = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		float GravityResetSpeed = 0.07f;

	UPROPERTY()
		bool CanDoFlag = true;

	//wall run angle (x)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		float WallRunInclinedAngle = -30.f;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayCameraShake();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		float ZForce = 6.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
		float XYForce = 1.f;
};
