// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSHUD.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "RTSMinion.h"
#include "RTSFPS/FPS/Commander.h"
#include "GameArchitecture/Game/RTFPSPlayerState.h"
#include "RTSFPS/RTS/Structures/RTSStructure.h"
#include "AssetHelpers/GameAssets.h"
#include "Runtime/Engine/Public/EngineUtils.h"

ARTSHUD::ARTSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT(FPS_CROSSHAIR_PATH));
	CrosshairTex = CrosshairTexObj.Object;

	state = GAME_INIT;
}

void ARTSHUD::DrawHUD() //similiar to "tick" of actor class overridden
{
	switch (state)
	{
	case ARTSHUD::GAME_INIT:
		break;
	case ARTSHUD::RTS_SELECT_AND_MOVE:
		RTSSelectAndMoveHandler();
		break;
	case ARTSHUD::FPS_AIM_AND_SHOOT:
		FPSAimAndShootHandler();
		break;
	case ARTSHUD::RTS_STRUCTURE_SELECT:
		RTSStructureSelectHandler();
		break;
	default:
		break;
	}

}

void ARTSHUD::GetActorsInSelectionRectangle(TSubclassOf<class AActor> ClassFilter, const FVector2D & FirstPoint, const FVector2D & SecondPoint, TArray<AActor*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed)
{
	// Because this is a HUD function it is likely to get called each tick,
	// so make sure any previous contents of the out actor array have been cleared!
	OutActors.Reset();

	//Create Selection Rectangle from Points
	FBox2D SelectionRectangle(ForceInit);

	//This method ensures that an appropriate rectangle is generated, 
	//		no matter what the coordinates of first and second point actually are.
	SelectionRectangle += FirstPoint;
	SelectionRectangle += SecondPoint;


	//The Actor Bounds Point Mapping
	const FVector BoundsPointMapping[8] =
	{
		FVector(1.f, 1.f, 1.f),
		FVector(1.f, 1.f, -1.f),
		FVector(1.f, -1.f, 1.f),
		FVector(1.f, -1.f, -1.f),
		FVector(-1.f, 1.f, 1.f),
		FVector(-1.f, 1.f, -1.f),
		FVector(-1.f, -1.f, 1.f),
		FVector(-1.f, -1.f, -1.f) };

	//~~~

	//For Each Actor of the Class Filter Type
	for (TActorIterator<AActor> Itr(GetWorld(), ClassFilter); Itr; ++Itr)
	{
		AActor* EachActor = *Itr;

		//Engine Line modified to get root of actor for selction
		const FBox EachActorBounds = EachActor->GetRootComponent()->Bounds.GetBox(); 

		//Center
		const FVector BoxCenter = EachActorBounds.GetCenter();

		//Extents
		const FVector BoxExtents = EachActorBounds.GetExtent();

		// Build 2D bounding box of actor in screen space
		FBox2D ActorBox2D(ForceInit);
		for (uint8 BoundsPointItr = 0; BoundsPointItr < 8; BoundsPointItr++)
		{
			// Project vert into screen space.
			const FVector ProjectedWorldLocation = Project(BoxCenter + (BoundsPointMapping[BoundsPointItr] * BoxExtents));
			// Add to 2D bounding box
			ActorBox2D += FVector2D(ProjectedWorldLocation.X, ProjectedWorldLocation.Y);
		}

		//Selection Box must fully enclose the Projected Actor Bounds
		if (bActorMustBeFullyEnclosed)
		{
			if (SelectionRectangle.IsInside(ActorBox2D))
			{
				OutActors.Add(EachActor);
			}
		}
		//Partial Intersection with Projected Actor Bounds
		else
		{
			if (SelectionRectangle.Intersect(ActorBox2D))
			{
				OutActors.Add(EachActor);
			}
		}
	}
}

void ARTSHUD::Change_HUD_State(int statetype)
{
	if (statetype > (int)LBOUND && statetype < (int)UBOUND)
	{
		state = (HUDSTATE)statetype;
	}
}

int ARTSHUD::Get_HUD_State()
{
	return ((int)state);
}

void ARTSHUD::RTSSelectAndMoveHandler()
{
	if (SelctionInProcess)
	{
		CleanSelectedActors();
		GetSelectedUnits();
	}
	else if (StructureSelected)
	{
		CleanSelectedActors();
		Selected_Units.Empty();
		GetSelectedStructures();
	}
}

void ARTSHUD::RTSStructureSelectHandler()
{

}

void ARTSHUD::FPSAimAndShootHandler()
{
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X),(Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void ARTSHUD::GetSelectedUnits()
{
	End_Select = GetMouseLocation();
	Selected_Units.Empty();
	Selected_Structure.Empty();
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(GetOwningPlayerController()->PlayerState);

	DrawRect(FLinearColor(0, 0, 1, selection_transparency), Initial_select.X, Initial_select.Y, End_Select.X - Initial_select.X, End_Select.Y - Initial_select.Y);
	GetActorsInSelectionRectangle<ARTSMinion>(Initial_select, End_Select, Selected_Units, false, false);

	if (Selected_Units.Num() > 0 && PS)
	{
		/*Save the size becasue its non static, and add on minions are accounted for in the loop*/
		int endindex = Selected_Units.Num();
		for (int32 i = 0; i < endindex; i++)
		{
			/*Remove Enemy Minions*/
			if (Selected_Units[i]->GetTeam() != PS->Team_ID)
			{
				Selected_Units.RemoveAt(i);
				endindex--;
				i--;
			}
			/*Unit has Commander, Get their squad and add it in*/
			else if(Selected_Units[i]->GetCommander())
			{
				/*Add the Commander*/
				Selected_Units.AddUnique(Selected_Units[i]->GetCommander());
				Selected_Units[i]->GetCommander()->SetSelected();

				/*Add His Squad*/
				for(int j = 0; j < Selected_Units[i]->GetCommander()->Squad.Num(); j++)
				{
					Selected_Units.AddUnique(Selected_Units[i]->GetCommander()->Squad[j]);
					Selected_Units[i]->GetCommander()->Squad[j]->SetSelected();
				}
				Selected_Units[i]->SetSelected();
			}
			else
			{
				Selected_Units[i]->SetSelected();
			}
		}
	}
}

FVector2D ARTSHUD::GetMouseLocation()
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);

	return(FVector2D(PosX, PosY));
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


