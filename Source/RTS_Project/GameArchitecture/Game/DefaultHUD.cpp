// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"


bool  ADefaultHUD::InitializeUI()
{
	return true;
}

UUserWidget* ADefaultHUD::GetPrimaryUI() const
{
	return MainUI;
}

void ADefaultHUD::SetPrimaryUI(UUserWidget* InUI)
{
	MainUI = InUI;
}
