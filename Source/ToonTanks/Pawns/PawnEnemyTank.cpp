// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemyTank.h"
#include "ToonTanks/Components/TankMovementComponent.h"

APawnEnemyTank::APawnEnemyTank()
{
	MovementComponent = CreateDefaultSubobject<UTankMovementComponent>(TEXT("Movement"));
}
float APawnEnemyTank::GetAcceptanceRadius() const
{
	return MovementComponent->AcceptanceRadius;
}