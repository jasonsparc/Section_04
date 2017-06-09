// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController not possessing a tank"))
	} else {
		UE_LOG(LogTemp, Warning, TEXT("TankPlayerController possesing: %s"), *ControlledTank->GetName())
	}
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick( DeltaSeconds );

	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // Out parameter
	if (GetSightRayHitLocation(HitLocation)) // Has "side-effect", is going to line trace
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}

// Get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	/// "De-project" the screen position of the crosshair to a world direction
	FVector LookLocation, LookDirection;

	if (GetLookDirection(LookLocation, LookDirection))
	{
		/// Line-trace along that LookDirection, and see what we hit (up to max range)
		FHitResult HitResult;
		auto StartTrace = PlayerCameraManager->GetCameraLocation();
		auto EndTrace = LookLocation + LookDirection * LineTraceRange;

		// DrawDebugLine(GetWorld(), LookLocation, EndTrace, FColor(0, 255, 0), false, -1, 1, 0.0f);
		// DrawDebugLine(GetWorld(), PlayerCameraManager->GetCameraLocation(), EndTrace, FColor(255, 0, 0), false, 0, 0, 0.0f);

		// UE_LOG(LogTemp, Warning, TEXT("Look location: %s"), *LookLocation.ToString());
		// UE_LOG(LogTemp, Warning, TEXT("Camera location: %s"), *PlayerCameraManager->GetCameraLocation().ToString());

		bool DidHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			LookLocation,
			EndTrace,
			ECollisionChannel::ECC_Visibility
		);

		if (DidHit) {
			OutHitLocation = HitResult.ImpactPoint;
			return true;
		}
	}

	OutHitLocation = FVector(0);
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector& LookLocation, FVector& LookDirection) const
{
	/// Find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	return DeprojectScreenPositionToWorld(
		ViewportSizeX * CrosshairXLocation,
		ViewportSizeY * CrosshairYLocation,
		LookLocation, LookDirection
	);
}
