// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "ToonTanks/Pawns/PawnBase.h"

void UTankMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<APawnBase>(GetOwner());
}

void UTankMovementComponent::Move(FVector MoveDirection)
{
	Owner->AddActorLocalOffset(MoveDirection, true);
}

void UTankMovementComponent::Rotate(FQuat RotationDirection)
{
	Owner->AddActorLocalRotation(RotationDirection, true);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FVector AITargetDirection = MoveVelocity.GetSafeNormal();
	FVector ActualForwardDirection = Owner->GetActorForwardVector().GetSafeNormal();
	
	// Calculate forward speed
	float Forward = FVector::DotProduct(AITargetDirection, ActualForwardDirection);
	FVector MoveDirection = FVector(
		/* X */ Forward * MoveSpeed * GetWorld()->DeltaTimeSeconds,
		/* Y */ 0,
		/* Z */ 0
	);

	// Calculate rotation speed
	float Yaw = FVector::CrossProduct(AITargetDirection, ActualForwardDirection).Z;
	FRotator RotationAmount = FRotator(
		/* Pitch: Y */ 0,
		/*  Yaw:  Z */ Yaw * RotationSpeed * GetWorld()->DeltaTimeSeconds,
		/*  Roll: X */ 0
	);

	Move(MoveDirection);

	Rotate(FQuat(RotationAmount));
}
