// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"
#include "Projectile.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UE_LOG(LogTemp, Warning, TEXT("%s DONKEY: Construct"), *GetName());
}

void ATank::BeginPlay()
{
	Super::BeginPlay(); // Needed for BP Begin Play to run!
	UE_LOG(LogTemp, Warning, TEXT("%s DONKEY: Begin Play"), *GetName());
}

void ATank::AimAt(const FVector & HitLocation)
{
	if (!TankAimingComponent) { return; }

	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
}

void ATank::Fire() {
	if (!Barrel) { return; }

	bool bIsReloaded = FPlatformTime::Seconds() - LastFireTime > ReloadTimeInSeconds;

	if (!bIsReloaded) { return; }

	// Spawn a projectile at the socket location of the barrel
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
		Barrel->GetSocketLocation(FName("Projectile")),
		Barrel->GetSocketRotation(FName("Projectile"))
	);

	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = FPlatformTime::Seconds();
}
