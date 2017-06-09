// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankAIController.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("TankAIController can't find player tank"))
	} else {
		UE_LOG(LogTemp, Warning, TEXT("TankAIController found player: %s"), *PlayerTank->GetName())
	}
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

	// Fire if ready
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
