// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true; // TODO Should this really tick?

	// ...
}

void UTankAimingComponent::AimAt(const FVector & HitLocation, float LaunchSpeed)
{
	if (!Barrel) { return; }
	
	FVector LaunchVelocity;
	// The socket location for "Projectile" or the component location if none otherwise
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool bHasAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		LaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false, // Chooses the Low Arc
		0, // Radius of the projectile/bullet (assumed spherical)
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	auto TankName = GetOwner()->GetName();
	if (bHasAimSolution)
	{
		auto AimDirection = LaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);

		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: [%s] Aim solution found"), Time, *TankName);
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: [%s] No aim solution found"), Time, *TankName);
	}
	// If no solution found do nothing
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet) {
	Barrel = BarrelToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Work-out difference between current barrel rotation, and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(5); // TODO Remove magic number
}
