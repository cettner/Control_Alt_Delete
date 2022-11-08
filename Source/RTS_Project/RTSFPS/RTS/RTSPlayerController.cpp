// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "EngineUtils.h"
 
void ARTSPlayerController::IssueDefaultOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	if (!HasAuthority())
	{
		ServerIssueDefaultOrder(InUnits, InHitContext, InbIsQueuedOrder);
	}
	else
	{
		for (int i = 0; i < InUnits.Num(); i++)
		{
			if (IsValid(InUnits[i].GetObject()))
			{
				const URTSOrder* defaultorder = InUnits[i]->GetDefaultOrder(InHitContext);
				InUnits[i]->IssueOrder(this, InHitContext, defaultorder, InbIsQueuedOrder);
			}
		}
	}
}

ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableClickEvents = true;
	bEnableAutoLODGeneration = true;
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	ClickEventKeys.Add(EKeys::RightMouseButton);
	ClickEventKeys.Add(EKeys::LeftMouseButton);
}

void ARTSPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	
	if (Cast<ADefaultPlayerState>(PlayerState) && Cast<ACommander>(InPawn))
	{
		int team_id = Cast<ADefaultPlayerState>(PlayerState)->GetTeamID();
		Cast<ACommander>(InPawn)->SetTeam(team_id);
	}	
}

void ARTSPlayerController::FinishLocalPlayerSetup()
{
}

TArray<IRTSObjectInterface*> ARTSPlayerController::GetOrderableUnits(TSubclassOf<AActor> InObjectClass) const
{
	TArray<IRTSObjectInterface*> retval = TArray<IRTSObjectInterface*>();
	const UWorld* world = GetWorld();
	const ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	/*this could in theory fail during inital replication setup, there is no time dependent function, so it is not called again from elsewhere*/
	if (IsValid(gs))
	{
		const TArray<IRTSObjectInterface*> registeredobjects = gs->GetRegisteredRTSObjects();
		for (int i = 0; i < registeredobjects.Num(); i++)
		{
			if (IsUnitOrderable(registeredobjects[i])  && CastChecked<UObject>(registeredobjects[i])->IsA(InObjectClass))
			{
				retval.Emplace(registeredobjects[i]);
			}
		}
	}

	return retval;
}

template<typename ClassFilter>
inline TArray<IRTSObjectInterface*> ARTSPlayerController::GetOrderableUnits()
{
	return GetOrderableUnits(ClassFilter::StaticClass());
}

bool ARTSPlayerController::IsUnitOrderable(const IRTSObjectInterface* InObj) const
{
	bool retval = IRTSObjectInterface::IsRTSObjectValid(InObj);

	if (retval == true && InObj->IsAlive())
	{
		retval &= (InObj->GetTeam() == GetTeamID());
	}
	return retval;
}

void ARTSPlayerController::IssueOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const URTSOrder* InOrder, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	if (!HasAuthority())
	{
		ServerIssueOrder(InUnits, InOrder);
	}
	else
	{
		for (int i = 0; i < InUnits.Num(); i++)
		{
			if (IsValid(InUnits[i].GetObject()))
			{
				InUnits[i]->IssueOrder(this, InHitContext, InOrder, InbIsQueuedOrder);	
			}

		}
	}
}

void ARTSPlayerController::ServerIssueOrder_Implementation(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnitsToOrder, const URTSOrder* InOrder, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	IssueOrder(InUnitsToOrder, InOrder, InHitContext, InbIsQueuedOrder);
}

bool ARTSPlayerController::ServerIssueOrder_Validate(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnitsToOrder, const URTSOrder* InOrder, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	return (true);
}

void ARTSPlayerController::ServerIssueDefaultOrder_Implementation(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	IssueDefaultOrder(InUnits, InHitContext, InbIsQueuedOrder);
}

bool ARTSPlayerController::ServerIssueDefaultOrder_Validate(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	return true;
}