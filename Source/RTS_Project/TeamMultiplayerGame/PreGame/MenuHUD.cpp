// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"


void AMenuHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	InitializeMenu();
}

void  AMenuHUD::InitializeMenu()
{
	if (MenuClass != nullptr)
	{
		CurrentMenu = CreateWidget(PlayerOwner, MenuClass);
	}
}