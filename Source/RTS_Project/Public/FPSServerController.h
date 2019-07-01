// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSPlayerController.h"
#include "FPSServerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AFPSServerController : public ARTSPlayerController
{
	GENERATED_BODY()

	UFUNCTION(Server, reliable, WithValidation)
	void Server_Request_Interact(ACommander * Controlled_Cmdr, AActor * Interacted);
	
	
};
