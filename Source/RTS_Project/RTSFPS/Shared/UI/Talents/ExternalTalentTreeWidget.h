// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UI/UpgradeTreeWidget.h"


#include "RTS_Project/GameArchitecture/Game/UI/Interfaces/ExternalWidgetInterface.h"
#include "ExternalTalentTreeWidget.generated.h"
/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UExternalTalentTreeWidget : public UUpgradeTreeWidget, public IExternalWidgetInterface
{
	GENERATED_BODY()

	protected:
		UExternalTalentTreeWidget();

	public:
		virtual FStackWidgetInfo GetWidgetInputSettings() const override;
		virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
		virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

	protected:
		virtual void OnEscapeAction();

	protected:
		UPROPERTY(EditDefaultsOnly)
		FStackWidgetInfo DefaultInputSettings = FStackWidgetInfo();

};
