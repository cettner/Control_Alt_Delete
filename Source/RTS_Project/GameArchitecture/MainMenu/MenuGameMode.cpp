// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "RTS_Project/PreGame/MenuPlayerController.h"
#include "RTS_Project/PreGame/MenuHUD.h"

AMenuGameMode::AMenuGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
}