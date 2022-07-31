// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

	protected:
		virtual void SetupInputComponent() override;
		virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;
};
