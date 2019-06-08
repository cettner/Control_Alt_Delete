// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultMode.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADefaultMode(const FObjectInitializer& ObjectInitializer);
	virtual void PostLogin(APlayerController * NewPlayer) override;
	virtual void Logout(AController * Exiting) override;
	
};
