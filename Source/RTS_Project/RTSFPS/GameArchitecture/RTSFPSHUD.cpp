// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSHUD.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"

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

void ARTSFPSHUD::SetGenrePlayType(EGenrePlayType InGenreType)
{
}

EGenrePlayType ARTSFPSHUD::GetGenrePlayType() const
{
	EGenrePlayType retval = EGenrePlayType::NONE;

	const ARTFPSPlayerState* ps = GetOwningPlayerController()->GetPlayerState<ARTFPSPlayerState>();

	if (ps != nullptr)
	{
		if (ps->IsRTSPlayer())
		{
			retval = EGenrePlayType::RTS;
		}
		else
		{
			retval = EGenrePlayType::FPS;
		}
	}

	return (retval);
}

bool ARTSFPSHUD::TryToggleUpgradeTree(EGenrePlayType InTreeToToggle)
{
	bool retval = false;
	UUpgradeTreeWidget * upgradetree = nullptr;

	if (InTreeToToggle == EGenrePlayType::AUTO)
	{
		const EGenrePlayType playtype = GetGenrePlayType();
		
		if (playtype == EGenrePlayType::RTS)
		{
			upgradetree = RTSUpgradeTree;
		}
		else if(InTreeToToggle == EGenrePlayType::FPS)
		{

			upgradetree = FPSUpgradeTree;
		}

	}
	else if (InTreeToToggle == EGenrePlayType::FPS)
	{
		upgradetree = FPSUpgradeTree;
	}
	else if (InTreeToToggle == EGenrePlayType::RTS)
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

	const EGenrePlayType playtype = GetGenrePlayType();

	if (playtype == EGenrePlayType::RTS)
	{
		upgradetree = RTSUpgradeTree;
	}
	else if (playtype == EGenrePlayType::FPS)
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
	APlayerController* pc = GetOwningPlayerController();

	if (FPSTalentTreeClass != nullptr)
	{
		FPSUpgradeTree = CreateWidget<UUpgradeTreeWidget>(pc, FPSTalentTreeClass);
	}
	
	if(RTSTalentTreeClass != nullptr)
	{
		RTSUpgradeTree = CreateWidget<UUpgradeTreeWidget>(pc, RTSTalentTreeClass);
	}


	return retval;
}

FStackWidgetInfo ARTSFPSHUD::GetDefaultInputSettings() const
{
	FStackWidgetInfo retval;
	const EGenrePlayType currentplaymode = GetGenrePlayType();

	if (currentplaymode == EGenrePlayType::RTS)
	{
		retval = DefaultInputSettings;
	}
	else
	{
		retval = FPSDefaultInput;
	}

	return retval;
}