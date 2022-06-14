// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

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

protected:
	UFUNCTION()
	virtual void OnHostSessionPressed();

	UFUNCTION()
	virtual void OnJoinSessionPressed();

protected:
	virtual ISessionMenuInterface* GetSessionInterface() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* SessionWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* JoinSessionButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* HostSessionButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget * HostSessionMenu = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UWidget * JoinSessionMenu = nullptr;


protected:
	ISessionMenuInterface* SessionMenuInterface = nullptr;
};
