// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TeamResourceSlotWidget.h"
#include "TeamResourcePanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UTeamResourcePanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	bool Initialize() override;


protected:
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ResourcePanel = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTeamResourceSlotWidget> ResourceSlotWidgetClass = nullptr;
};
