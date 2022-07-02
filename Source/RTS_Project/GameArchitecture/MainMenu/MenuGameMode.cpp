// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "RTS_Project/PreGame/MenuPlayerController.h"

AMenuGameMode::AMenuGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}