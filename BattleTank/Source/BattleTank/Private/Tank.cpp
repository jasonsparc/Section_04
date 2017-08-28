// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FMath::RoundToInt(DamageAmount); /// Actually calls FPlatformMath::RoundToInt
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	///UE_LOG(LogTemp, Warning, TEXT("DamageAmount=%f, DamageToApply=%i"), DamageAmount, DamageToApply);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tank died"));
	}
	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return float(CurrentHealth) / float(StartingHealth);
}
