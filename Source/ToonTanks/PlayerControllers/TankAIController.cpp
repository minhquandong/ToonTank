// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "GameFramework/Pawn.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnEnemyTank.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerTank = Cast<APawnTank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	GameMode = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	EnemyPawn = Cast<APawnEnemy>(PawnTank);
	
	FTimerHandle StartMovementTimerHandle;
	float DelayTime;
	
	if (GameMode->bIsSurvivalMode) DelayTime = 0.3f;
	else DelayTime = GameMode->GetStartDelay();
	
	GetWorld()->GetTimerManager().SetTimer(StartMovementTimerHandle, this, &ATankAIController::EnableMovement, DelayTime, true);
}

void ATankAIController::Tick(float Delta)
{
	Super::Tick(Delta);
	if (!bCanMove) return;
	if (!PawnTank || !PlayerTank || !PlayerTank->bIsPlayerAlive) return;
	
	if (GetDistanceToTarget() <= EnemyPawn->ScanRadius)
	{
		MoveToActor(PlayerTank, PawnTank->GetAcceptanceRadius());
	}
	
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	PawnTank = Cast<APawnEnemyTank>(InPawn);
}

void ATankAIController::EnableMovement()
{
	bCanMove = true;
}

float ATankAIController::GetDistanceToTarget()
{
	return FVector::Dist(PlayerTank->GetActorLocation(), PawnTank->GetActorLocation());
}
