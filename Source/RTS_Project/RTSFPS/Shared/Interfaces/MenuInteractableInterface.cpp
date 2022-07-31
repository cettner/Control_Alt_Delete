

// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuInteractableInterface.h"

// Add default functionality here for any IMenuInteractableInterface functions that are not pure virtual.

UUserWidget* IMenuInteractableInterface::GetMenu()
{
	return(nullptr);
}


bool IMenuInteractableInterface::CanOpenMenu(APawn* InvokingPawn) const
{
	return(InvokingPawn && InvokingPawn->IsLocallyControlled());
}


void IMenuInteractableInterface::OpenMenu()
{
}

void IMenuInteractableInterface::CloseMenu()
{
}
