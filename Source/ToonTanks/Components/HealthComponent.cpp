// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = BaseHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	GameMode = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if(!GameMode)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode not found!"));
		return;
	}

	if (Health <= 0) return;

	Health = FMath::Clamp<float>(Health - Damage, 0.f, MaxHealth);

	if (Health <= 0) 
	{
		GameMode->ActorDied(Cast<APawnBase>(GetOwner()));
	}
}

float UHealthComponent::GetRemainingHealthPercent() const
{
	return UKismetMathLibrary::SafeDivide(Health, MaxHealth);
}