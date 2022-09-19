// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RTSSelectionPanelWidget.h"
#include "Properties/RTSPropertyPanelWidget.h"
#include "RTSUI.generated.h"

UCLASS()
class RTS_PROJECT_API URTSUI : public UUserWidget
{
	GENERATED_BODY()

public:
	URTSPropertyPanelWidget* GetRTSPropertyPanel() const;
	URTSSelectionPanelWidget* GetRTSSelectionPanel() const;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	URTSPropertyPanelWidget* RTSPropertyPanel = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	URTSSelectionPanelWidget* RTSSelectionPanel = nullptr;
};
