// Fill out your copyright notice in the Description page of Project Settings.


#include "qbplayer.h"
#include "defs.h"
#include "gameframework/charactermovementcomponent.h"
#include "kismet/gameplaystatics.h"

// Sets default values
Aqbplayer::Aqbplayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	wallruncomp = CreateDefaultSubobject<UWallRunComponent>(TEXT("WallRunComponent"));

}

// Called when the game starts or when spawned
void Aqbplayer::BeginPlay()
{
	Super::BeginPlay();
	dashes_left = playerDashCount;
	GetWorld()->GetTimerManager().SetTimer(dashTimer, this, &Aqbplayer::AddDashCharge, playerDashReplenishTime, true, -1.f);

	if (IsValid(rifle)) {
		rifle_rot = GetActorRotation() + rifle->GetActorRotation();
		rifle_recoil_pos = rifle_rot + recoil_added;
	}
	
}

// Called every frame
void Aqbplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetCharacterMovement()->IsFalling() == false) {
		jumped_in_air = false;
		armor = 10.f;
	}
	else {
		armor = 20.f;
	}

	if (IsValid(rifle)) {
		rifle->SetActorRotation(FMath::RInterpConstantTo(rifle->GetActorRotation(), GetControlRotation(), DeltaTime, recoil_intensity * 1.5f));
	}

	float curr_velo = GetVelocity().Size();
	if (curr_velo > max_velocity) {
		curr_velo = max_velocity;
	}
	current_velocity_score = FMath::FInterpTo(current_velocity_score, curr_velo, DeltaTime, 1.f);

	float added_dmg = (current_velocity_score / velo_div);
	if (IsValid(rifle)) {
		rifle->added_damage = added_dmg;
	}

}

// Called to bind functionality to input
void Aqbplayer::SetupPlayerInputComponent(UInputComponent* pc)
{
	Super::SetupPlayerInputComponent(pc);
	pc->BindAxis("moveforward", this, &Aqbplayer::MoveForward);
	pc->BindAxis("moveright", this, &Aqbplayer::MoveRight);
	pc->BindAxis("lookup", this, &Aqbplayer::LookUp);
	pc->BindAxis("turn", this, &Aqbplayer::Turn);

	pc->BindAction("jump", EInputEvent::IE_Pressed, this, &Aqbplayer::_Jump);
	//pc->BindAction("jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);

	pc->BindAction("mouseleft", EInputEvent::IE_Pressed, this, &Aqbplayer::PrimaryAbilityStart);
	pc->BindAction("mouseleft", EInputEvent::IE_Released, this, &Aqbplayer::PrimaryAbilityStop);

	pc->BindAction("mouseright", EInputEvent::IE_Pressed, this, &Aqbplayer::SecondaryAbilityStart);
	pc->BindAction("mouseright", EInputEvent::IE_Released, this, &Aqbplayer::SecondaryAbilityStop);


	pc->BindAction("Dash", EInputEvent::IE_Pressed, this, &Aqbplayer::Dash);
	//pc->BindAction("Dash", EInputEvent::IE_Released, this, &Aqbplayer::StopDash);
	pc->BindAction("Dilation", EInputEvent::IE_Pressed, this, &Aqbplayer::TimeDilation);
	pc->BindAction("Grapple", EInputEvent::IE_Pressed, this, &Aqbplayer::Grapple);


}

void Aqbplayer::OnDeath() {
	Super::OnDeath();
}


void Aqbplayer::MoveForward(float Value) {
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
	dashForwardVal = Value;
}
void Aqbplayer::MoveRight(float Value) {
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
	dashRightVal = Value;
}
void Aqbplayer::LookUp(float Value) {
	AddControllerPitchInput(Value);
}
void Aqbplayer::Turn(float Value) {
	AddControllerYawInput(Value);
}

void Aqbplayer::PrimaryAbilityStart()
{
	if (!IsValid(rifle)) return;
	rifle->ActivateAbility();
}

void Aqbplayer::PrimaryAbilityStop()
{
	if (!IsValid(rifle)) return;
	rifle->DeactivateAbility();
}

void Aqbplayer::Gun_Fired_CB() {
	if (IsValid(rifle)) {
		// Add recoil on the pitch axis (you can adjust the value)

		// Apply recoil in local space

		FRotator NewRotation = rifle->GetActorRotation() + recoil_added;

		// Set the new rotation
		rifle->SetActorRotation(NewRotation);
	}
}

void Aqbplayer::Dash() {
	//GetCharacterMovement()->MaxWalkSpeed = 900.f;
	if (dashes_left == 0) return;
	if ((GetGameTimeSinceCreation() - last_dash_at) <= 0.1f) return;
	last_dash_at = GetGameTimeSinceCreation();
	FVector force_dir = GetActorRightVector() * dashRightVal + GetActorForwardVector() * dashForwardVal;
	force_dir.Normalize();
	bool isfalling = GetCharacterMovement()->IsFalling();
	GetCharacterMovement()->Velocity += force_dir * (isfalling ? playerDashForceFalling : playerDashForce);
	GetWorld()->GetTimerManager().ClearTimer(stopDashTimer);
	GetWorld()->GetTimerManager().SetTimer(stopDashTimer, this, &Aqbplayer::StopDash, 1.5f, false, -1.f);
	dashes_left--;


}
void Aqbplayer::StopDash() {
	//GetCharacterMovement()->MaxWalkSpeed = 600.f;
	if (GetCharacterMovement()->IsFalling() == false)
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void Aqbplayer::AddDashCharge() {
	dashes_left++;
	if (dashes_left > playerDashCount) {
		dashes_left = playerDashCount;
	}
}

void Aqbplayer::_Jump() {
	if ((GetGameTimeSinceCreation() - last_dash_at) <= 0.1f) return;
	if (GetCharacterMovement()->IsFalling()) {
		if(jumped_in_air) return;
		jumped_in_air = true;
	}
	last_dash_at = GetGameTimeSinceCreation();
	FVector dir = GetActorUpVector();
	float force = playerJumpForce;
	if (wallruncomp->AreWeWallRunning) {
		dir = GetActorForwardVector();
		force *= 1.5f;
	}
	last_dash_at = GetGameTimeSinceCreation();
	LaunchCharacter(dir * playerJumpForce, false, false);
}

void Aqbplayer::TimeDilation() {
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), dilationQuant);
	GetWorld()->GetTimerManager().SetTimer(dilationTimer, this, &Aqbplayer::EndDilation, dilationTime, false, -1);
}

void Aqbplayer::EndDilation() {
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void Aqbplayer::Grapple() {
	return;
	if (!CanGrapple()) return;
	last_grapple_at = GetGameTimeSinceCreation();
	OnGrapple();

	FVector grappleloc;
	FRotator grapplerot;
	GetActorEyesViewPoint(grappleloc, grapplerot);
	FHitResult hit;
	FCollisionQueryParams qp;
	qp.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hit, grappleloc, grappleloc + grapplerot.Vector() * 15000.f, ECollisionChannel::ECC_Camera, qp);
	if (hit.bBlockingHit) {
		FVector launchdir = (hit.ImpactPoint - GetActorLocation());
		launchdir.Normalize();
		LaunchCharacter(launchdir * grappleLaunchForce, false, false);
	}

}

bool Aqbplayer::CanGrapple() {
	return ((GetGameTimeSinceCreation() - last_grapple_at) > grappleCooldown);
}


void Aqbplayer::SecondaryAbilityStart() {
	if (IsValid(force_gun) && force_gun->IsAbilityOffCooldown()) {
		//prints("using force gun");
		force_gun->ActivateAbility();
		FVector grappleloc;
		FRotator grapplerot;
		GetActorEyesViewPoint(grappleloc, grapplerot);
		FHitResult hit;
		FCollisionQueryParams qp;
		qp.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(hit, grappleloc, grappleloc + grapplerot.Vector() * 15000.f, ECollisionChannel::ECC_Camera, qp);
		if (hit.bBlockingHit) {
			FVector launchdir = (hit.ImpactPoint - GetActorLocation());
			launchdir.Normalize();
			LaunchCharacter(-1.f * launchdir * grappleLaunchForce, false, false);
		}
	}
}

void Aqbplayer::SecondaryAbilityStop() {

}