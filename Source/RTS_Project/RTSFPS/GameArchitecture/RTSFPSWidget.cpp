// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSWidget.h"

#include "RTFPSPlayerState.h"

URTSFPSWidget::URTSFPSWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FPSUIClass = UFPSUI::StaticClass();
	RTSUIClass = URTSUI::StaticClass();
	RTSFPSUISwitcher = CreateDefaultSubobject<UWidgetSwitcher>(TEXT("RTSFPSUISwitcher"));
}

bool URTSFPSWidget::Initialize()
{
	bool retval =  Super::Initialize();

	if (RTSFPSUISwitcher == false) return false;

	FPSUI = CreateWidget<UFPSUI>(this, FPSUIClass, "FPSUI");
	RTSFPSUISwitcher->AddChild(FPSUI);

	RTSUI = CreateWidget<URTSUI>(this, RTSUIClass, "RTSUI");
	RTSFPSUISwitcher->AddChild(RTSUI);

	OnHUDStateChange();

	return true;
}

void URTSFPSWidget::OnHUDStateChange()
{
	const ARTFPSPlayerState * ps = GetOwningPlayer()->GetPlayerState<ARTFPSPlayerState>();

	if (ps->IsRTSPlayer())
	{
		RTSFPSUISwitcher->SetActiveWidget(RTSUI);
	}
	else
	{
	    RTSFPSUISwitcher->SetActiveWidget(FPSUI);
	}
}

UFPSUI* URTSFPSWidget::GetFPSUI() const
{
	return FPSUI;
}

URTSUI* URTSFPSWidget::GetRTSUI() const
{
	return RTSUI;
}

UWidget* URTSFPSWidget::GetCurrentUI() const
{
	UWidget * retval = RTSFPSUISwitcher->GetActiveWidget();
	return retval;
}
