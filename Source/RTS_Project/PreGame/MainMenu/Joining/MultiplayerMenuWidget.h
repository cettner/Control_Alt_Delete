// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "RTS_Project/LobbySystem/Interfaces/SessionMenuInterface.h"
#include "MultiplayerMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMultiplayerMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual bool Initialize() override;
	virtual void Setup(ISessionMenuInterface* InSessionInterface);

	UPROPERTY(meta = (BindWidget))
	UButton* HostSessionButton;

	UFUNCTION()
	virtual void OnHostSessionPressed();

	UPROPERTY(meta = (BindWidget))
	UButton* JoinSessionButton;

	UFUNCTION()
	virtual void OnJoinSessionPressed();

	virtual ISessionMenuInterface* GetSessionInterface();

protected:
	ISessionMenuInterface* SessionMenuInterface = nullptr;
};
