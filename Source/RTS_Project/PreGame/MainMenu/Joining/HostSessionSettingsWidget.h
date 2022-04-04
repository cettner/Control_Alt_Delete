// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "HostSessionSettingsWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UHostSessionSettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	


protected:
	/*User Input TODO:: Make Setable from UI*/
	UPROPERTY(EditDefaultsOnly, Category = Session)
	FString DesiredServerName;

	UPROPERTY(EditDefaultsOnly, Category = Session)
	int PlayersPerTeam;
};
