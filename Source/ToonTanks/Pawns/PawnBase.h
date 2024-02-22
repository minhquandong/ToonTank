// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class UCapsuleComponent;
class AProjectileBase;
class UHealthComponent;
class UWidgetComponent;
UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	APawnBase();

	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction();

	UFUNCTION(BlueprintCallable)
	float GetHealthBarValue() const;

	UFUNCTION(BlueprintCallable)
	float CalculateDamage(int32 Level);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 PowerLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseDamage = 50.f;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseFireRate = 1.6f;

	void RotateTurret(FVector TargetLocation);

	virtual void Fire();

	virtual void CreateDefaultSubobjects();

	virtual void SetupAttachments();

	virtual bool HasNullPointers();

	void RotateWidgetTowardsPlayerCamera(UWidgetComponent* WidgetComponent);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

private:
	/*================================ COMPONENTS ====================================*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthWidgetComponent;

	/*================================ VARIABLES ====================================*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound = nullptr;
};
