// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameArchitecture/Instance/DefaultGameInstance.h"
#include "DefaultPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UFUNCTION(Server, reliable, WithValidation)
	void ServerRegisterPlayerInfo(FPlayerSettings settings);

	virtual void RegisterPlayerInfo(FPlayerSettings settings);
};
