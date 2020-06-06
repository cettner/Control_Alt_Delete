// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "PreGame\MainMenu\MainMenuPlayerController.h"

AMenuGameMode::AMenuGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = AMainMenuPlayerController::StaticClass();
}