// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

APawnBase::APawnBase()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateDefaultSubobjects();
	SetupAttachments();
	CalculateDamage(PowerLevel);
}

void APawnBase::CreateDefaultSubobjects()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
}

void APawnBase::SetupAttachments()
{
	RootComponent = CapsuleComponent;
	BaseMesh->SetupAttachment(RootComponent);
	TurretMesh->SetupAttachment(BaseMesh);
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
	HealthWidgetComponent->SetupAttachment(RootComponent);
}

void APawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	RotateWidgetTowardsPlayerCamera(HealthWidgetComponent);
}

void APawnBase::RotateWidgetTowardsPlayerCamera(UWidgetComponent* WidgetComponent)
{
	FRotator FaceCameraRotation = UKismetMathLibrary::FindLookAtRotation(
		WidgetComponent->GetComponentLocation(),									// Widget location
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation()	// Camera location

	);
	WidgetComponent->SetWorldRotation(FaceCameraRotation);
}

void APawnBase::RotateTurret(FVector TargetLocation)
{
	FVector TurretLocation = GetActorLocation();
	TurretLocation.Z = TargetLocation.Z; // Eliminate rotation in Z axis
	FRotator RotationDirection = UKismetMathLibrary::FindLookAtRotation(TurretLocation, TargetLocation);

	TurretMesh->SetWorldRotation(RotationDirection);
}

void APawnBase::Fire()
{
	if (HasNullPointers()) return;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	Projectile->SetOwner(this);					// Prevent the projectile to damage the Pawn
	Projectile->ProjectileDamage = Damage;		// Set Damage for the projectile
}

void APawnBase::HandleDestruction()
{
	if (HasNullPointers()) return;

	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

}

float APawnBase::GetHealthBarValue() const
{
	return HealthComponent->GetRemainingHealthPercent();
}

float APawnBase::CalculateDamage(int32 CurrentPowerLevel)
{
	Damage = BaseDamage * (0.2f * CurrentPowerLevel + 1);
	return Damage;
}

bool APawnBase::HasNullPointers() 
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileClass not found!"));
		return true;
	}

	if (!DeathParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("DeathParticle not found!"));
		return true;
	}

	if (!DeathSound)
	{
		UE_LOG(LogTemp, Error, TEXT("DeathSound not found!"));
		return true;
	}

	return false;
}
