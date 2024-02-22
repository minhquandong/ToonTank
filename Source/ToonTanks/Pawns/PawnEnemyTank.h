// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnEnemy.h"
#include "PawnEnemyTank.generated.h"

class UTankMovementComponent;

UCLASS()
class TOONTANKS_API APawnEnemyTank : public APawnEnemy
{
	GENERATED_BODY()

public:
	APawnEnemyTank();
	float GetAcceptanceRadius() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTankMovementComponent* MovementComponent = nullptr;
	
};
