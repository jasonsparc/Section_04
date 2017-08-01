// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankPlayerController.h"
#include "TankAimingComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();
	if (AimingComponent) {
		FoundAimingComponent(AimingComponent);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Player controller can't find aiming component at Begin Play"))
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

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetPawn());

		bool DidHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			LookLocation,
			EndTrace,
			ECollisionChannel::ECC_Visibility,
			QueryParams
		);

		if (DidHit) {
			OutHitLocation = HitResult.ImpactPoint;
			return true;
		}
	}

	OutHitLocation = FVector(0);
	return true;
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
