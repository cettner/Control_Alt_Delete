// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"

#include "EngineUtils.h"


template<typename ClassFilter>
inline bool ARTSHUD::GetActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<ClassFilter*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed)
{
	//Is Actor subclass?
	if (!ClassFilter::StaticClass()->IsChildOf(AActor::StaticClass()))
	{
		return false;
	}

	//Run Inner Function, output to Base AActor Array
	TArray<AActor*> OutActorsBaseArray;
	GetActorsInSelectionRectangle(ClassFilter::StaticClass(), FirstPoint, SecondPoint, OutActorsBaseArray, bIncludeNonCollidingComponents, bActorMustBeFullyEnclosed);

	//Construct casted template type array
	for (AActor* EachActor : OutActorsBaseArray)
	{
		OutActors.Add(CastChecked<ClassFilter>(EachActor));
	}

	return true;
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

void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (IsBoxSelectionEnabled())
	{
		RTSSelectAndMoveHandler();
	}

}

FVector2D ARTSHUD::GetMouseLocation() const
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	return(FVector2D(PosX, PosY));
}

bool ARTSHUD::ClientInitializeHUD()
{
	bool retval = Super::ClientInitializeHUD();

	const EGenrePlayType playtype = GetGenrePlayType();

	if (playtype == EGenrePlayType::RTS)
	{
		SetBoxSelectionEnabled(true);
	}


	return retval;
}

void ARTSHUD::RTSSelectAndMoveHandler()
{
	if (bIsSelectionInProcess)
	{
		CleanSelectedActors();
		GetSelectedUnits();
	}
}

void ARTSHUD::GetSelectedUnits()
{
	FVector2D End_Select = GetMouseLocation();
	Selected_Units.Empty();
	Selected_Structure.Empty();
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(GetOwningPlayerController()->PlayerState);

	DrawRect(FLinearColor(0, 0, 1, SelectionAlpha), Initial_select.X, Initial_select.Y, End_Select.X - Initial_select.X, End_Select.Y - Initial_select.Y);
	GetActorsInSelectionRectangle<ARTSMinion>(Initial_select, End_Select, Selected_Units, false, false);

	if (Selected_Units.Num() > 0 && PS)
	{
		/*Save the size becasue its non static, and add on minions are accounted for in the loop*/
		int endindex = Selected_Units.Num();
		for (int32 i = 0; i < endindex; i++)
		{
			/*Remove Minions that are dead, dieing, or being removed from memory*/
			if (!Selected_Units[i] ||  !Selected_Units[i]->IsAlive())
			{
				Selected_Units.RemoveAt(i);
				endindex--;
				i--;
			}
			/*Remove Enemy Minions*/
			else if (Selected_Units[i]->GetTeam() != PS->TeamID)
			{
				Selected_Units.RemoveAt(i);
				endindex--;
				i--;
			}
			/*Unit is Commander, Get their squad and add it in*/
			else if(ACommander * cmdr = Cast<ACommander>(Selected_Units[i]->GetLeadRTSObject()))
			{
				/*Add the Commander*/
				Selected_Units.AddUnique(cmdr);
				cmdr->SetSelected();

				/*Add His Squad*/
				for(int j = 0; j < cmdr->Squad.Num(); j++)
				{
					Selected_Units.AddUnique(cmdr->Squad[j]);
					cmdr->Squad[j]->SetSelected();
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
		for (int i = 0; i < Selected_Units.Num(); i++)
		{
			Selected_Units[i]->SetDeselected();
		}
	}
}

bool ARTSHUD::IsBoxSelectionEnabled() const
{
	return bIsBoxSelectEnabled;
}

void ARTSHUD::SetBoxSelectionEnabled(const bool InEnabled)
{
	bIsBoxSelectEnabled = InEnabled;

}



