// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSHUD.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "RTSMinion.h"
#include "RTSStructure.h"


ARTSHUD::ARTSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void ARTSHUD::DrawHUD() //similiar to "tick" of actor class overridden
{

		if (SelctionInProcess)
		{
			CleanSelectedActors();
			GetSelectedUnits();
		}
		else if (StructureSelected)
		{
			CleanSelectedActors();
			GetSelectedStructures();
		}
}

FVector2D ARTSHUD::GetMouseLocation()
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX,PosY);

	return(FVector2D(PosX, PosY));
}

void ARTSHUD::GetSelectedUnits()
{
	End_Select = GetMouseLocation();
	Selected_Units.Empty();
	DrawRect(FLinearColor(0, 0, 1, selection_transparency), Initial_select.X, Initial_select.Y,
		End_Select.X - Initial_select.X, End_Select.Y - Initial_select.Y);

	GetActorsInSelectionRectangle<ARTSMinion>(Initial_select, End_Select, Selected_Units, false, false);

	if (Selected_Units.Num() > 0)
	{
		for (int32 i = 0; i < Selected_Units.Num(); i++)
		{
			Selected_Units[i]->SetSelected();
		}
	}
}

void ARTSHUD::GetSelectedStructures()
{
	if (Selected_Structure.Num() > 0)
	{
		for (int32 i = 0; i < Selected_Structure.Num(); i++)
		{
			Selected_Structure[i]->SetSelected();
		}
	}
}

void ARTSHUD::CleanSelectedActors()
{
	if (Selected_Units.Num() > 0)
	{
		for (int32 i = 0; i < Selected_Units.Num(); i++)
		{
			Selected_Units[i]->SetDeselected();
		}
	}
	if (Selected_Structure.Num() > 0)
	{
		for (int32 i = 0; i < Selected_Structure.Num(); i++)
		{
			Selected_Structure[i]->SetDeselected();
		}
	}

}

