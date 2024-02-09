// Fill out your copyright notice in the Description page of Project Settings.


#include "WallRunComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/Array.h"

// Sets default values for this component's properties
UWallRunComponent::UWallRunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	WallRunButtonPressed = true;
	// ...
}


// Called when the game starts
void UWallRunComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacter>(GetOwner());
	if(Player != nullptr) {
		PlayerController = Player->GetController();
		MovementComponent = Player->GetCharacterMovement();
		if(PlayerController != nullptr) {
			DefaultControlRotation = PlayerController->GetControlRotation();
			//GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Good controller"));
		}
	}
	
}

// Called every frame
void UWallRunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(Player == nullptr || PlayerController == nullptr || MovementComponent == nullptr || GetWorld() == nullptr) return;

	/*Local Variables*/
	float Velocity = Player->GetVelocity().Size();
	FRotator CurrentControlRotation = PlayerController->GetControlRotation();
	FVector TraceStartLocation = Player->GetActorLocation();
	FVector TraceRightEndLocation = TraceStartLocation + Player->GetActorRightVector() * TraceDistance;
	FVector TraceLeftEndLocation = TraceStartLocation + Player->GetActorRightVector() * TraceDistance * -1.f;
	FHitResult RightHit;
	FHitResult LeftHit;
	FHitResult Hit; // Will be used
	bool HasRightHit = false;
	bool HasLeftHit = false;

	bool IsFalling = MovementComponent->IsFalling();

	if(WallRunOver) {
		/*Wall Running Over */
		MovementComponent->GravityScale = 1.f;
		FRotator TargetRotation = CurrentControlRotation;
		TargetRotation.Roll = DefaultControlRotation.Roll;
		PlayerController->SetControlRotation(FMath::RInterpTo(CurrentControlRotation, TargetRotation, DeltaTime, InterpResetSpeed));

		if(FMath::Abs(PlayerController->GetControlRotation().Roll - DefaultControlRotation.Roll) <= 0.5f) {
			ResetAllVariables();
		}

	} else {
		/*Still Wall Running*/
		if(WallRunButtonPressed) {
			if(IsFalling) {
				/*Trace both sides*/
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
				ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
				TArray<AActor*> IgnoredActors;
				IgnoredActors.Add(Player);
				
				HasRightHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStartLocation, TraceRightEndLocation, 
					ObjectTypes, false, IgnoredActors, EDrawDebugTrace::None, RightHit, true);
				
				HasLeftHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), TraceStartLocation, TraceLeftEndLocation,
					ObjectTypes, false, IgnoredActors, EDrawDebugTrace::None, LeftHit, true);

				/*Setting wall and hit result*/
			} else {
				//ambiguous call to reset
				WallRunOver = AreWeWallRunning;
			}
			if (!HasRightHit && HasLeftHit) {
				Hit = LeftHit;
				WallSelection = -1;
			}
			else if (!HasLeftHit && HasRightHit) {
				Hit = RightHit;
				WallSelection = 1;
			}
			else if (HasLeftHit && HasRightHit) {
				if (LeftHit.Distance < RightHit.Distance) {
					WallSelection = -1;
					Hit = LeftHit;
				}
				else {
					WallSelection = 1;
					Hit = RightHit;
				}
			}	else {
				WallSelection = 0;
				//ambiguous
				//WallRunOver = true;
				if (AreWeWallRunning) {
					WallRunOver = true;
				}
			}
			if(WallSelection != 0) {
				CanWallRun = true;
			}
			/*if(WallSelection != 0) GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, TEXT("WallFound"));
			else GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, TEXT("No Wall Found"));*/
			/*Angle Check*/
			if (CanWallRun && WallSelection != 0) {
				float DotProd = FVector::DotProduct(Player->GetActorForwardVector(), Hit.ImpactNormal);
				if (DotProd >= -1.5f && DotProd <= 0.3) {
					if (Velocity >= 300.f) {
						CanWallRun = true;
					}	else {
						//WallRunOver = true;
						CanWallRun = false;
						if (AreWeWallRunning) {
							WallRunOver = true;
							//Low player velocity
						}
					}
				}	else {
					//Bad start angle
					CanWallRun = false;
				}
			}
			//*Launching character and setting cam angle*/
			if(CanWallRun) {
				if(Velocity <= 1000.f) {
					//Launch only if velocity is < 1000
					float Magnitude = (Velocity >= 900.f ? 150.f : 40.f);
					float Div = Velocity / Magnitude;
					FVector LForward = FVector::CrossProduct(Player->GetActorUpVector() * WallSelection, Hit.Normal);
					LForward.Normalize();
					LForward *= Div;
					Player->LaunchCharacter(LForward,  false, false);
					// launched
				} //else GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Not Launched"));
				AreWeWallRunning = true;
				PlayCameraShake();
				//see launch interp event in blueprint
				if(CanDoFlag) {
					CanDoFlag = 0;
					MovementComponent->GravityScale = CustomGravityScale;
				}

				//reset gravity back to normal
				MovementComponent->GravityScale = FMath::FInterpTo(MovementComponent->GravityScale, 1.f, DeltaTime, GravityResetSpeed);
				//world cam shake?

				FRotator TargetRot = PlayerController->GetControlRotation();
				TargetRot.Roll = WallSelection * WallRunInclinedAngle;
				PlayerController->SetControlRotation(FMath::RInterpTo(PlayerController->GetControlRotation(), TargetRot, DeltaTime, 4.f));

				//Additional Force -- Can be skipped
				FVector LaunchVector = (Hit.ImpactPoint - Player->GetActorLocation()) * XYForce;
				LaunchVector.Z = ZForce;
				//Player->LaunchCharacter(LaunchVector, false, false);
			}

		} else {
			return;
		}
	}
}

void UWallRunComponent::ResetAllVariables()
{
	WallRunOver = false;
	AreWeWallRunning = false;
	CanWallRun = false;
	MovementComponent->GravityScale = 1.f;
	WallSelection = 0;
	CanDoFlag = 1;
	//BP is setting gravity to 1 here, check
}

