// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"

#include "ToonTanks/Components/TankMovementComponent.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

APawnTank::APawnTank()
{
	CreateDefaultSubobjects();

	BaseFireRate = 1.15f;
	MovementComponent->MoveSpeed = 470.f;
}

void APawnTank::CreateDefaultSubobjects()
{
	SpringArmPomponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmPomponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmPomponent);

	MovementComponent = CreateDefaultSubobject<UTankMovementComponent>(TEXT("Movement"));

	ReloadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Reload Widget"));
	ReloadWidgetComponent->SetupAttachment(RootComponent);
}

void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	PlayerLevel = 1;
	CurrentExperience = 0.f;
	CalculateMaxExperience(PlayerLevel);
	CalculateFireRate(AttackSpeedLevel);
	CalculateDamage(PowerLevel);

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasNullPointers()) return;

	RotateTurret(GetCursorPositionInTheWorld().ImpactPoint);
}

FHitResult APawnTank::GetCursorPositionInTheWorld()
{
	FHitResult CursosHitResult; //Out
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursosHitResult);
	return CursosHitResult;
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APawnTank::Move);
		EnhancedInputComponent->BindAction(Attack, ETriggerEvent::Triggered, this, &APawnTank::Fire);
	}
}

void APawnTank::Move(const FInputActionValue& AxisValue)
{
	
	FVector MovementVector = AxisValue.Get<FVector>();
	FVector MoveForward = MovementVector.GetSafeNormal() * MovementComponent->MoveSpeed * GetWorld()->DeltaTimeSeconds;
	FVector CorrectDirection(MoveForward.Y, MoveForward.X, 0);		// Swap X and Y
	
	if (Controller != nullptr && MovementComponent && MovementComponent->UpdatedComponent == RootComponent)
	{
		MovementComponent->Move(CorrectDirection);
	}
	
	// Rotate Base toward move direction
	FRotator NewRotation = FRotationMatrix::MakeFromX(CorrectDirection).Rotator();
	if (CorrectDirection.Length() > 0)
	{
		RotateBase(NewRotation);
	}
}

void APawnTank::RotateBase(FRotator NewRotation)
{
	FRotator Rotator = FRotator(0.f, NewRotation.Yaw, 0.f);				// Keep it rotating arround Z-axis only

	BaseMesh->SetWorldRotation(
		FMath::RInterpTo(
			BaseMesh->GetComponentRotation(),
			Rotator,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			BaseTurnRate)
	);
}

void APawnTank::HandleDestruction()
{
	Super::HandleDestruction();
	
	bIsPlayerAlive = false;
	SetActorHiddenInGame(true);	// Hide mesh
	SetActorTickEnabled(false); // Stop moving
}


void APawnTank::Fire()
{
	if (HasNullPointers()) return;
	if (GetWorld()->GetTimeSeconds() < NextShotReadyAt) return;
	NextShotReadyAt = GetWorld()->GetTimeSeconds() + FireRate;

	Super::Fire();
}

float APawnTank::GetReloadBarValue() const
{
	return FMath::Clamp<float>(1 - ((NextShotReadyAt - GetWorld()->GetTimeSeconds()) / FireRate), 0.f, 1.f);
}

void APawnTank::AddXP(float XPGained)
{
	CurrentExperience = CurrentExperience + XPGained;
}

float APawnTank::CalculateMaxExperience(int32 Level)
{
	MaxExperience = ceil(pow(Level / 0.2f, 1.3f) / 5) * 5;    // Formula: (PlayerLevel/X)^Y  then round up to nearest 5
	return MaxExperience;
}

float APawnTank::CalculateFireRate(int32 CurrentAttackSpeedLevel)
{
	FireRate = BaseFireRate / (1 + CurrentAttackSpeedLevel * 0.2f);
	return FireRate;
}

bool APawnTank::HasNullPointers()
{
	if (Super::HasNullPointers()) return true;
	if (!PlayerController) {
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
		return true;
	}
	return false;
}