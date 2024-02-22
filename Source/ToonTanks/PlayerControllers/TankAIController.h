// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class APawnTank;
class APawnEnemy;
class APawnEnemyTank;
class ATankGameModeBase;

UCLASS()
class TOONTANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void Tick(float Delta) override;
	void SetPawn(APawn* InPawn) override;

private:
	APawnTank* PlayerTank = nullptr;
	APawnEnemy* EnemyPawn = nullptr;
	APawnEnemyTank* PawnTank = nullptr;
	ATankGameModeBase* GameMode = nullptr;
	bool bCanMove = false;
	
	void EnableMovement();

	float GetDistanceToTarget();
};
