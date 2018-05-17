// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSHUD.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "RTSMinion.h"


ARTSHUD::ARTSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void ARTSHUD::DrawHUD() //similiar to "tick" of actor class overridden
{
	if (bIsRTSCharacter)
	{
		if (SelctionInProcess)
		{
			if (Selected_Units.Num() > 0)
			{
				for (int32 i = 0; i < Selected_Units.Num(); i++)
				{
					Selected_Units[i]->SetDeselected();
				}
			}

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
	}

	else
	{
		// Find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// Offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5)),
			(Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));

		// Draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

FVector2D ARTSHUD::GetMouseLocation()
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX,PosY);

	return(FVector2D(PosX, PosY));
}

