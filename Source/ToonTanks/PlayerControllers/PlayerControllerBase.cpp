// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

void APlayerControllerBase::SetControllerEnabled(bool ControllerEnabled)
{
	bShowMouseCursor = ControllerEnabled;

	if (ControllerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
}