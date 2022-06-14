// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"

#include "HostSessionSettingsWidget.generated.h"




UCLASS()
class RTS_PROJECT_API UHostSessionSettingsWidget : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY(meta = (BindWidget))
	UButton * BeginHostingButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UEditableTextBox* ServerNameEditTextBox = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UEditableTextBox* PasswordEditTextBox = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UCheckBox* PrivateGameCheckBox = nullptr;



protected:
	virtual bool Initialize() override;

protected:
	UFUNCTION()
	virtual void OnBeginHostingButtonPressed();

protected:
	
	FString DesiredServerName = "Default Server Name";

	bool bAllowSpectators = false;

	int NumAllowedSpectators = 3;

	bool bIsPrivateGame = false;

	
};
