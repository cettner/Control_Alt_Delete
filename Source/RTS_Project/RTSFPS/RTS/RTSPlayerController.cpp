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
		/*Currently only Uobjects can be passed via RPC, so have to obtain the object portion on the client then rebuild interface structs on the server*/
		TArray<UObject*> replicationobjects = TArray<UObject*>();
		for (int i = 0; i < InUnits.Num(); i++)
		{
			replicationobjects.Emplace(InUnits[i].GetObject());
		}
		ServerIssueDefaultOrder(replicationobjects, InHitContext, InbIsQueuedOrder);
	}
	else
	{
		UWorld* world = GetWorld();
		ARTFPSMode* gm = world->GetAuthGameMode<ARTFPSMode>();
		gm->GetOrderManager()->BuildOrderGroup(InUnits, InHitContext, this);

	}
}

void ARTSPlayerController::IssueOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, URTSOrder* InOrder, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	if (!HasAuthority())
	{
		/*Currently only Uobjects can be passed via RPC, so have to obtain the object portion on the client then rebuild on the server*/
		TArray<UObject*> replicationobjects = TArray<UObject*>();
		for (int i = 0; i < InUnits.Num(); i++)
		{
			replicationobjects.Emplace(InUnits[i].GetObject());
		}
		ServerIssueOrder(replicationobjects, InOrder);
	}
	else
	{
		for (int i = 0; i < InUnits.Num(); i++)
		{
			if (IsValid(InUnits[i].GetObject()))
			{
				//InUnits[i]->IssueOrder(this, InHitContext, InOrder, InbIsQueuedOrder);
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
	/*Init Enemy Selection, This is done here instead of the playerstate because the Minions need to call GetPlayerState(), 
	which returns null on inital playerstate replication*/
	ARTSPlayerState* ps = Cast<ARTSPlayerState>(PlayerState);
	ps->InitEnemySelection();
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

	if (retval == true)
	{
		retval &= (InObj->GetTeam() == GetTeamID());
	}
	return retval;
}

void ARTSPlayerController::ServerIssueOrder_Implementation(const TArray<UObject*>& InUnitsToOrder, URTSOrder* InOrder, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	TArray<TScriptInterface<IRTSObjectInterface>> interfaceobjects = TArray<TScriptInterface<IRTSObjectInterface>>();
	for (int i = 0; i < InUnitsToOrder.Num(); i++)
	{
		interfaceobjects.Emplace(TScriptInterface<IRTSObjectInterface>(InUnitsToOrder[i]));
	}
	IssueOrder(interfaceobjects, InOrder, InHitContext, InbIsQueuedOrder);
}

bool ARTSPlayerController::ServerIssueOrder_Validate(const TArray<UObject*>& InUnitsToOrder, URTSOrder* InOrder, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	return (true);
}

void ARTSPlayerController::ServerIssueDefaultOrder_Implementation(const TArray<UObject*>& InUnits, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	TArray<TScriptInterface<IRTSObjectInterface>> interfaceobjects = TArray<TScriptInterface<IRTSObjectInterface>>();
	for (int i = 0; i < InUnits.Num(); i++)
	{
		interfaceobjects.Emplace(TScriptInterface<IRTSObjectInterface>(InUnits[i]));
	}
	IssueDefaultOrder(interfaceobjects, InHitContext, InbIsQueuedOrder);
}

bool ARTSPlayerController::ServerIssueDefaultOrder_Validate(const TArray<UObject*>& InUnits, const FHitResult InHitContext, const bool InbIsQueuedOrder)
{
	return true;
}