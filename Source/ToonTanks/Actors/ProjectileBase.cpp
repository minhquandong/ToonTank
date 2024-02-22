// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetupComponents();
}

void AProjectileBase::SetupComponents()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = StaticMeshComponent;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));

	ParticleTrailComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrailComponent->SetupAttachment(RootComponent);

	ProjectileComponent->InitialSpeed = ProjectileSpeed;
	ProjectileComponent->MaxSpeed = ProjectileSpeed;
	ProjectileComponent->ProjectileGravityScale = 0;
	InitialLifeSpan = 1.5f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit); // Macro for add dynamic delegate. See OnComponentHit for more information

	if (HasNullPointer()) return;
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;
	if (!OtherActor || OtherActor == this || OtherActor == MyOwner) return; // Do not damage self
	
	UGameplayStatics::ApplyDamage(OtherActor, ProjectileDamage, MyOwner->GetInstigatorController(), this, DamageType);
	PlayEffects();
	Destroy();
}

void AProjectileBase::PlayEffects() const
{
	if (HasNullPointer()) return;
	// Sound
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	// Particles
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
}

bool AProjectileBase::HasNullPointer() const
{
	if (!StaticMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("StaticMeshComponent not found!"));
		return true;
	}
	if (!ProjectileComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileComponent not found!"));
		return true;
	}
	if (!ParticleTrailComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ParticleTrailComponent not found!"));
		return true;
	}

	if (!LaunchSound)
	{
		UE_LOG(LogTemp, Error, TEXT("LaunchSound not found!"));
		return true;
	}
	if (!HitParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("HitParticle not found!"));
		return true;
	}
	if (!HitSound)
	{
		UE_LOG(LogTemp, Error, TEXT("HitSound not found!"));
		return true;
	}
	return false;
}


