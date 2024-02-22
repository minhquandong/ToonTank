// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnEnemy.h"

#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

void APawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	GameMode = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(this));

	UpdateEnemyLevel();
	SetTimerHandle();
	HealthComponent->Health = HealthComponent->MaxHealth;
}

void APawnEnemy::SetTimerHandle()
{
	FTimerHandle FireRateTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnEnemy::CheckFireCondition, BaseFireRate, true);
}

bool APawnEnemy::HasNullPointers()
{
	if (Super::HasNullPointers()) return true;
	if (!PlayerPawn) {
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn not found!"));
		return true;
	}
	return false;
}


void APawnEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasNullPointers()) return;
	
	if (GetDistanceToPawn() <= AggroRange)
	{
		RotateTurret(PlayerPawn->GetActorLocation());
	}

	// Destroy if the pawn is too far away from player
	if (GameMode->bIsSurvivalMode && GetDistanceToPawn() > 3000.f)
	{
		Destroy();
	}
		
}

void APawnEnemy::CheckFireCondition()
{
	if (HasNullPointers()) return;

	if (!PlayerPawn->bIsPlayerAlive) return;

	if (GetDistanceToPawn() > FireRange) return;

	Fire();
}

float APawnEnemy::GetDistanceToPawn()
{
	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnEnemy::HandleDestruction()
{
	Super::HandleDestruction();

	PlayerPawn->AddXP(Experience);
	GameMode->CalculateScore(Score);
	Destroy();
}

void APawnEnemy::UpdateEnemyLevel()
{
	float BaseHealth = HealthComponent->BaseHealth;

	EnemyLevel = PlayerPawn->PlayerLevel;
	HealthComponent->MaxHealth = pow(EnemyLevel-1, 2) * BaseHealth / 150 + BaseHealth;		// (Level^2 * BaseHP / 150) + BaseHP
	HealthComponent->Health = HealthComponent->MaxHealth;
}