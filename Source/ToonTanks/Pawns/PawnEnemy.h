// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnEnemy.generated.h"

class APawnTank;
class ATankGameModeBase;

UCLASS()
class TOONTANKS_API APawnEnemy : public APawnBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;

	UPROPERTY(EditAnywhere, category = "Combat")
	float ScanRadius = 1600.f;

protected:
	virtual void BeginPlay() override;

	virtual bool HasNullPointers() override;

private:
	APawnTank* PlayerPawn = nullptr;
	ATankGameModeBase* GameMode = nullptr;

	void SetTimerHandle();

	void CheckFireCondition();
	void UpdateEnemyLevel();
	float GetDistanceToPawn();

	UPROPERTY(EditAnywhere, category = "Combat")
	float FireRange = 650.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Experience = 5.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Score = 100;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 EnemyLevel = 1;
};
