// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"

#include "RTS_Project/LobbySystem/Interfaces/SessionMenuInterface.h"
#include "HostSessionSettingsWidget.generated.h"




UCLASS()
class RTS_PROJECT_API UHostSessionSettingsWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual bool Initialize() override;
	virtual void InitSessionInterface();

protected:
	UFUNCTION()
	virtual void OnBeginHostingButtonPressed();

	UFUNCTION()
	virtual void OnPrivateGameCheckStateChanged(bool InState);

public:
	ISessionMenuInterface * GetSessionInterface() const;
	virtual FString GetServerName() const;
	virtual FString GetDefaultServerName() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BeginHostingButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UEditableTextBox* ServerNameEditTextBox = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UEditableTextBox* PasswordEditTextBox = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UCheckBox* PrivateGameCheckBox = nullptr;

protected:
	
	FString DefaultServerName = "Default Server Name";

	bool bAllowSpectators = false;

	int NumAllowedSpectators = 3;

	bool bIsPrivateGame = false;

protected:

	ISessionMenuInterface * SessionInterface = nullptr;
};
