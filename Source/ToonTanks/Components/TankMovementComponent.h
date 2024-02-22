// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class APawnBase;

UCLASS()
class TOONTANKS_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	

public:
	void Move(FVector MoveDirection);
	void Rotate(FQuat RotationDirection);

	void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptanceRadius = 450.f;

protected:
	virtual void BeginPlay() override;

private:
	APawnBase* Owner = nullptr;
};
