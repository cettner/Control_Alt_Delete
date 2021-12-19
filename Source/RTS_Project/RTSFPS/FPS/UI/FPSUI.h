// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "RTS_Project/RTSFPS/Shared/UI/TeamResourceWidget.h"
#include "FPSUI.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFPSUI : public UUserWidget
{
	GENERATED_BODY()

    public:
		UFPSUI(const FObjectInitializer& ObjectInitializer);

	protected:
		virtual bool Initialize() override;

    protected:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UTeamResourceWidget> ResourceWidgetClass;

    protected:
	   UPROPERTY(meta = (BindWidgetOptional))
	   UPanelWidget* TeamResourceList;
};
