// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileDamage = 10.f;

protected:
	virtual void BeginPlay() override;

private:
	/*========================== COMPONENTS ==================================*/
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UProjectileMovementComponent* ProjectileComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UParticleSystemComponent* ParticleTrailComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	/*========================== VARIABLES ==================================*/
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ProjectileSpeed = 1300.f;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* LaunchSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* HitSound = nullptr;


	/*========================== FUNCTIONS ==================================*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void PlayEffects() const;

	void SetupComponents();

	bool HasNullPointer() const;

};
