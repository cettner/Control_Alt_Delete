// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "EngineUtils.h"


template<typename ClassFilter>
inline bool ARTSHUD::GetRTSActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<IRTSObjectInterface*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed)
{
	//Run Inner Function, output to Base AActor Array
	TArray<AActor*> OutActorsBaseArray;
	GetRTSActorsInSelectionRectangle(FirstPoint, SecondPoint, OutActorsBaseArray, bIncludeNonCollidingComponents, bActorMustBeFullyEnclosed, ClassFilter::StaticClass());

	//Construct casted template type array
	for (AActor* EachActor : OutActorsBaseArray)
	{
		OutActors.Add(CastChecked<IRTSObjectInterface>(EachActor));
	}

	return true;
}

void ARTSHUD::GetRTSActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed, TSubclassOf<AActor> InActorClass)
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

	ARTFPSGameState* gs = GetWorld()->GetGameState<ARTFPSGameState>();
	checkf(gs,TEXT("ARTSHUD::GetRTSActorsInSelectionRectangle : Failed to obtain Gamestate"))
	const TArray<IRTSObjectInterface*> allobs = gs->GetRegisteredRTSObjects();

	//For Each Actor of the Class Filter Type
	for (int i = 0;i < allobs.Num(); i++)
	{
		AActor* EachActor = Cast<AActor>(allobs[i]);
		if (IsValid(EachActor) && EachActor->IsA(InActorClass) && allobs[i]->IsBoxSelectable())
		{
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
}

void ARTSHUD::StartRTSSelection(const FVector2D InStartScreenPosition, const TArray<IRTSObjectInterface*>& StartingSelection)
{
	if (!bIsSelectionInProcess)
	{
		for (int i = 0; i < StartingSelection.Num(); i++)
		{
			RetainedSelection.AddUnique(StartingSelection[i]);
		}
		SelectionStartPosition = InStartScreenPosition;
		bIsSelectionInProcess = true;
	}

}

bool ARTSHUD::IsSelectionInProgress() const
{
	return bIsSelectionInProcess;
}

TArray<IRTSObjectInterface*> ARTSHUD::FinishRTSSelection()
{
	TArray<IRTSObjectInterface*> retval;
	
	if (bIsSelectionInProcess)
	{
		retval = Selected_Units;
		
		/*Add in the previous selection if we've attempted to retain additional units from a different selection*/
		for (int i = 0; i < RetainedSelection.Num();)
		{
			retval.AddUnique(RetainedSelection[i]);
		}
		Selected_Units.Reset();
		RetainedSelection.Reset();
		bIsSelectionInProcess = false;
	}
	else
	{
		retval = TArray<IRTSObjectInterface*>();
	}

	return  retval;
}

bool ARTSHUD::ForceRemoveSelection(IRTSObjectInterface* InObject)
{
	int32 removal = 0;
	removal += Selected_Units.Remove(InObject);
	removal += RetainedSelection.Remove(InObject);
	return (removal > 0);
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
		ScanSelectedUnits();
	}
}

void ARTSHUD::ScanSelectedUnits()
{
	const FVector2D End_Select = GetMouseLocation();
	Selected_Units.Reset();
	const ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(GetOwningPlayerController()->PlayerState);

	/*Draw a Rectangle */
	DrawRect(FLinearColor(0, 0, 1, SelectionAlpha), SelectionStartPosition.X, SelectionStartPosition.Y, End_Select.X - SelectionStartPosition.X, End_Select.Y - SelectionStartPosition.Y);
	GetRTSActorsInSelectionRectangle<AActor>(SelectionStartPosition, End_Select, Selected_Units, false, false);

	for (int i = 0; i < RetainedSelection.Num(); i++)
	{
		Selected_Units.Emplace(RetainedSelection[i]);
	}

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
			else if (Selected_Units[i]->GetTeam() != PS->GetTeamID())
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



