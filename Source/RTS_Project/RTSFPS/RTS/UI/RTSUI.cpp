// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSUI.h"

URTSSelectionPanelWidget* URTSUI::GetRTSSelectionPanel() const
{
	return RTSSelectionPanel;
}

bool URTSUI::Initialize()
{
	const bool retval = Super::Initialize();
	if (IsValid(RTSPropertyPanel) && IsValid(RTSSelectionPanel))
	{
		RTSPropertyPanel->Setup(RTSSelectionPanel->SelectedPropertiesDelegate);
	}

	return retval;
}

URTSPropertyPanelWidget* URTSUI::GetRTSPropertyPanel() const
{
	return RTSPropertyPanel;
}