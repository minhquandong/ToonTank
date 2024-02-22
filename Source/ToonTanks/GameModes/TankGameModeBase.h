// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

class APawnBase;
class APlayerControllerBase;

UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(APawnBase* DeadActor);

	UFUNCTION(BlueprintCallable)
	int32 GetRemainingEnemiesCounter() const;

	int32 GetStartDelay() const;

	/********* Setup Survival Mode *********/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	bool bIsSurvivalMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	int32 GameMinutes = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
	int32 GameSeconds = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 GameScore = 0;

	int32 CalculateScore(int32 Value);

	void TimeCountdown();

	UFUNCTION(BlueprintCallable)
	FText GetScore(int32 Value);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);


	/********* Setup Survival Mode *********/
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayScore(int32 Score);

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayGameTimer(int32 Minutes, int32 Seconds);

private:
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

	void SetupVariables();
	bool MissingPlayerController() const;
	void EnablePlayerControllerWithDelay();
	void EnableGameCountdownTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode", meta = (AllowPrivateAccess = "true"))
	int32 StartDelay = 3;

	APawnBase* PlayerPawn = nullptr;

	APlayerControllerBase* PlayerController = nullptr;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundBase* Music = nullptr;

	int32 RemainingEnemies = 0;

};
