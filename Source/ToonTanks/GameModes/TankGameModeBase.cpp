// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetupVariables();
	HandleGameStart();
	GameScore = 0;
}

void ATankGameModeBase::SetupVariables()
{
	PlayerPawn = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	RemainingEnemies = GetRemainingEnemiesCounter();
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ATankGameModeBase::HandleGameStart()
{
	EnablePlayerControllerWithDelay();

	GameStart(); // BlueprintImplementableEvent

	if (Music) UGameplayStatics::PlaySound2D(this, Music);
}

void ATankGameModeBase::EnablePlayerControllerWithDelay()
{
	if (MissingPlayerController()) return;
	PlayerController->SetControllerEnabled(false); // Disable input

	FTimerHandle EnableControllerTimerHandle; // Create Time Manager and Delegate to reenable the controller
	FTimerDelegate EnableControllerDelegate = FTimerDelegate::CreateUObject(PlayerController, &APlayerControllerBase::SetControllerEnabled, true);
	GetWorld()->GetTimerManager().SetTimer(EnableControllerTimerHandle, EnableControllerDelegate, StartDelay, false);

	// Enable Time Countdown for Survival Mode
	if (bIsSurvivalMode)
	{
		FTimerHandle EnableCountDownTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EnableCountDownTimerHandle, this, &ATankGameModeBase::EnableGameCountdownTimer, 1.0f, false, StartDelay);
		//GetWorldTimerManager().ClearTimer(EnableControllerTimerHandle);
	}
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	GameOver(PlayerWon); // BlueprintImplementableEvent
}

void ATankGameModeBase::ActorDied(APawnBase* DeadActor)
{
	DeadActor->HandleDestruction();
	if(DeadActor == PlayerPawn)
	{
		HandleGameOver(false);
		//if (!MissingPlayerController()) PlayerController->SetControllerEnabled(false); // Disable input
		return;
	}
	
	if (!bIsSurvivalMode)
	{
		if (--RemainingEnemies <= 0) HandleGameOver(true);
	}
}

int32 ATankGameModeBase::GetRemainingEnemiesCounter() const
{
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnEnemy::StaticClass(), EnemyActors);
	return EnemyActors.Num();
}

bool ATankGameModeBase::MissingPlayerController() const
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
		return true;
	}
	return false;
}

int32 ATankGameModeBase::GetStartDelay() const
{
	return StartDelay;
}



/********* Setup Survival Mode *********/
void ATankGameModeBase::EnableGameCountdownTimer()
{
	FTimerHandle GameCountdownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(GameCountdownTimerHandle, this, &ATankGameModeBase::TimeCountdown, 1.0f, true, 1.0f);
}

void ATankGameModeBase::TimeCountdown()
{
	if (GameSeconds > 0)
	{
		--GameSeconds;
	}
	else
	{
		--GameMinutes;
		GameSeconds = 59;
		if (GameMinutes <= 0)
		{
			GameSeconds = 0;
			HandleGameOver(true);
		}
	}

	DisplayGameTimer(GameMinutes, GameSeconds);
}

FText ATankGameModeBase::GetScore(int32 Value)
{
	return FText();
}

int32 ATankGameModeBase::CalculateScore(int32 Value)
{
	GameScore = GameScore + Value;
	DisplayScore(GameScore);
	return GameScore;
}