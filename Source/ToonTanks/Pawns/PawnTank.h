// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "InputActionValue.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTankMovementComponent;
class UWidgetComponent;

UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
	GENERATED_BODY()

public:
	APawnTank();

	/*======================== FUNCTIONS ================================*/
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void HandleDestruction() override;

	UFUNCTION(BlueprintCallable)
	float GetReloadBarValue() const;

	void AddXP(float XPGained);

	UFUNCTION(BlueprintCallable)
	float CalculateMaxExperience(int32 Level);

	UFUNCTION(BlueprintCallable)
	float CalculateFireRate(int32 AttackSpeedLevel);

	/*======================== VARIABLES ================================*/
	UPROPERTY(BlueprintReadOnly)
	bool bIsPlayerAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 PlayerLevel = 1;

	float MoveScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 AttackSpeedLevel = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	float FireRate;

protected:
	virtual void BeginPlay() override;

	virtual void Fire() override;

	virtual bool HasNullPointers() override;


private:
	/*======================== COMPONENTS ================================*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmPomponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTankMovementComponent* MovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* ReloadWidgetComponent = nullptr;
	
	APlayerController* PlayerController = nullptr;

	/*======================== VARIABLES ================================*/
	float NextShotReadyAt = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 8.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CurrentExperience = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxExperience;

	/*======================== FUNCTIONS ================================*/
	void Move(const FInputActionValue& AxisValue);

	void RotateBase(FRotator NewRotation);

	void CreateDefaultSubobjects();

	FHitResult GetCursorPositionInTheWorld();

	/*======================== INPUTS ================================*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Attack;
};
