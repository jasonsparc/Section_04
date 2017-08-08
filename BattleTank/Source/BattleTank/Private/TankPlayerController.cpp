// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"
#include "TankAimingComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	// See, https://answers.unrealengine.com/questions/538020/4141-playercontroller-tick-function-being-called-w.html
	if (GetWorld()->WorldType == EWorldType::EditorPreview) return;

	Super::Tick( DeltaSeconds );

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // Out parameter
	if (GetSightRayHitLocation(HitLocation)) // Has "side-effect", is going to line trace
	{
		AimingComponent->AimAt(HitLocation);
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
