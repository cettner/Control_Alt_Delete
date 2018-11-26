// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSHUD.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "RTSMinion.h"
#include "RTSStructure.h"
#include "Runtime/Engine/Public/EngineUtils.h"

ARTSHUD::ARTSHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	state = RTS_SELECT_AND_MOVE;
}

void ARTSHUD::DrawHUD() //similiar to "tick" of actor class overridden
{
	switch (state)
	{
	case ARTSHUD::RTS_SELECT_AND_MOVE:
		RTSSelectAndMoveHandler();
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
		GetSelectedStructures();
	}
}

void ARTSHUD::RTSStructureSelectHandler()
{

}

void ARTSHUD::GetSelectedUnits()
{
	End_Select = GetMouseLocation();
	Selected_Units.Empty();
	Selected_Structure.Empty();
	DrawRect(FLinearColor(0, 0, 1, selection_transparency), Initial_select.X, Initial_select.Y,
		End_Select.X - Initial_select.X, End_Select.Y - Initial_select.Y);

	GetActorsInSelectionRectangle<ARTSMinion>(Initial_select, End_Select, Selected_Units, false, false);

	if (Selected_Units.Num() > 0)
	{
		for (int32 i = 0; i < Selected_Units.Num(); i++)
		{
			
			if (Selected_Units[i]->team_index != GetWorld()->GetControllerIterator().GetIndex())
			{
				Selected_Units.RemoveAt(i);
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



