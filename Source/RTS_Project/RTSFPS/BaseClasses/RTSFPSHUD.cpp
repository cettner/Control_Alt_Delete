// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSHUD.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSPlayerState.h"

ARTSFPSHUD::ARTSFPSHUD() : Super()
{
	/*Can't See Cursor */
	FPSDefaultInput.bIsCursorVisible = false;
	/*Don't Care*/
	FPSDefaultInput.CursorType = EMouseCursor::Default;
	FPSDefaultInput.InputType = EWisgetStackInputType::GAMEONLY;
	FPSDefaultInput.StackSettings = EWidgetStackOperation::ALWAYSACTIVE;

	DefaultInputSettings.bIsCursorVisible = true;
	DefaultInputSettings.CursorType = EMouseCursor::Crosshairs;
	DefaultInputSettings.InputType = EWisgetStackInputType::GAMEONLY;
	DefaultInputSettings.StackSettings = EWidgetStackOperation::ALWAYSACTIVE;
}

void ARTSFPSHUD::DrawHUD() //similiar to "tick" of actor class overridden
{
	switch (state)
	{
	case HUDSTATE::GAME_INIT:
		break;
	case HUDSTATE::RTS_SELECT_AND_MOVE:
		RTSSelectAndMoveHandler();
		break;
	case HUDSTATE::FPS_AIM_AND_SHOOT:
		FPSAimAndShootHandler();
		break;
	case HUDSTATE::RTS_STRUCTURE_SELECT:
		RTSStructureSelectHandler();
		break;
	default:
		break;
	}

}

void ARTSFPSHUD::ChangeHUDState(HUDSTATE statetype)
{
	if (statetype > LBOUND&& statetype < UBOUND)
	{
		state = statetype;
		URTSFPSWidget* mainui = GetPrimaryUI<URTSFPSWidget>();
		mainui->OnHUDStateChange();
	}
}

HUDSTATE ARTSFPSHUD::GetHUDState() const
{
	return (state);
}

FVector2D ARTSFPSHUD::GetMouseLocation() const
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	return(FVector2D(PosX, PosY));
}

bool ARTSFPSHUD::TryToggleUpgradeTree(ETalentTreeTypes InTreeToToggle)
{
	bool retval = false;
	UUpgradeTreeWidget * upgradetree = nullptr;

	if (InTreeToToggle == ETalentTreeTypes::AUTO)
	{
		const HUDSTATE currentstate = GetHUDState();
		
		if (currentstate == HUDSTATE::RTS_SELECT_AND_MOVE || currentstate == HUDSTATE::RTS_STRUCTURE_SELECT)
		{
			upgradetree = RTSUpgradeTree;
		}
		else if (currentstate == HUDSTATE::FPS_AIM_AND_SHOOT || currentstate == HUDSTATE::FPS_AWAITING_RESPAWN)
		{
			upgradetree = FPSUpgradeTree;
		}

	}
	else if (InTreeToToggle == ETalentTreeTypes::FPS && IsValid(FPSUpgradeTree))
	{
		upgradetree = FPSUpgradeTree;
	}
	else if (InTreeToToggle == ETalentTreeTypes::RTS && IsValid(RTSUpgradeTree))
	{
		upgradetree = RTSUpgradeTree;
	}

	const bool treeisvalid = IsValid(upgradetree);
	if (treeisvalid == true)
	{
		PushExternalWidget(upgradetree);
		retval = true;
	}

	return retval;
}

void ARTSFPSHUD::RefreshUpgradeTree()
{
	UUpgradeTreeWidget* upgradetree = nullptr;
	const HUDSTATE currentstate = GetHUDState();

	if (currentstate == HUDSTATE::RTS_SELECT_AND_MOVE || currentstate == HUDSTATE::RTS_STRUCTURE_SELECT)
	{
		upgradetree = RTSUpgradeTree;
	}
	else if (currentstate == HUDSTATE::FPS_AIM_AND_SHOOT || currentstate == HUDSTATE::FPS_AWAITING_RESPAWN)
	{
		upgradetree = FPSUpgradeTree;
	}

	if (IsValid(upgradetree))
	{
		upgradetree->RefreshNodes();
	}
}

bool ARTSFPSHUD::ClientInitializeHUD()
{
	bool retval = Super::ClientInitializeHUD();

	const ARTFPSPlayerState* ps = GetOwningPlayerController()->GetPlayerState<ARTFPSPlayerState>();
	
	if (!ps->IsRTSPlayer())
	{
		ChangeHUDState(HUDSTATE::FPS_AIM_AND_SHOOT);
	}
	else
	{
		ChangeHUDState(HUDSTATE::RTS_SELECT_AND_MOVE);
	}

	if (FPSTalentTreeClass != nullptr)
	{
		FPSUpgradeTree = CreateWidget<UUpgradeTreeWidget>(PlayerOwner, FPSTalentTreeClass);
	}
	
	if(RTSTalentTreeClass != nullptr)
	{
		RTSUpgradeTree = CreateWidget<UUpgradeTreeWidget>(PlayerOwner, RTSTalentTreeClass);
	}


	return retval;
}

FStackWidgetInfo ARTSFPSHUD::GetDefaultInputSettings() const
{
	FStackWidgetInfo retval;
	const HUDSTATE currentstate = GetHUDState();
	const bool isrtsmode = (currentstate == RTS_SELECT_AND_MOVE) || (currentstate == RTS_STRUCTURE_SELECT);

	if (isrtsmode == true)
	{
		retval = DefaultInputSettings;
	}
	else
	{
		retval = FPSDefaultInput;
	}

	return retval;
}

void ARTSFPSHUD::RTSSelectAndMoveHandler()
{
}

void ARTSFPSHUD::RTSStructureSelectHandler()
{
}

void ARTSFPSHUD::FPSAimAndShootHandler()
{
}
