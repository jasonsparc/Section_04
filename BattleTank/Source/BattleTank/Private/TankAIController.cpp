// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankAIController.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto ControlledTank = GetControlledTank();
	if (!ControlledTank) { return; }

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) { return; }

	// TODO Move towards the player

	ControlledTank->AimAt(PlayerTank->GetActorLocation());

	// Aim towards the player

	// TODO Fire only when ready
	ControlledTank->Fire();
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
