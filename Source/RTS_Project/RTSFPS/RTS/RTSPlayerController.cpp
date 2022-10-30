// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "EngineUtils.h"
 
ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableClickEvents = true;
	bEnableAutoLODGeneration = true;
	FOWManagerClass = AFogOfWarManager::StaticClass();
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

AFogOfWarManager * ARTSPlayerController::InitFOW()
{
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(PlayerState);

	/*Server Will spawn the FOWManager*/
	if (!FOWManager)
	{
		FActorSpawnParameters SpawnParams;
		UWorld * World = GetWorld();

		if (World && FOWManagerClass)
		{
			FOWManager = World->SpawnActor<AFogOfWarManager>(FOWManagerClass, SpawnParams);
		}
	}

	return(FOWManager);
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

bool ARTSPlayerController::MoveMinions_Validate(ARTSPlayerController * PC, const TArray<ARTSMinion *> &Units, FHitResult Hit)
{
	return (true);
}

void ARTSPlayerController::MoveMinions_Implementation(ARTSPlayerController * PC, const TArray<ARTSMinion *> &Units, FHitResult Hit)
{
	AActor * target = Hit.GetActor();

	for (int i = 0; i < Units.Num(); i++)
	{
		if (!IsValid(Units[i]) || !Units[i]->IsAlive()) /*Skip Dead or Invalid Minions*/
		{

		}
		else if (!Units[i]->GetLeadRTSObject()) /*Unit is or has a commander, notify him instead*/
		{

			if (Cast<IRTSObjectInterface>(target))
			{
				Units[i]->SetTarget(target);
			}
			else
			{
				FVector MoveLocal = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

				Units[i]->ClearTarget(); /*Unit might be doing something, if he is, clear internal data*/
				Units[i]->ReleaseAssets();
				Units[i]->SetTargetLocation(MoveLocal);
			}
		}
		else //Notify the Commander of the new Target
		{

		}
	}
}

bool ARTSPlayerController::ServerPurchaseRTSObject_Validate(ARTSStructure * SpawningStructure, TSubclassOf<UObject> RequestedClass)
{
	return(true);
}

void ARTSPlayerController::ServerPurchaseRTSObject_Implementation(ARTSStructure * SpawningStructure, TSubclassOf<UObject> RequestedClass)
{
	//SpawningStructure->PurchaseUnit()
}

bool ARTSPlayerController::ServerPurchaseStructure_Validate(TSubclassOf<AActor> RequestedClass, FTransform BuildLocation)
{
	return(true);
}

void ARTSPlayerController::ServerPurchaseStructure_Implementation(TSubclassOf<AActor> RequestedClass, FTransform BuildLocation)
{
	UWorld* world = GetWorld();
	check(world);

	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();

	//if (gs && gs->PurchaseUnit(RequestedClass,this))
	//{
	//	gs->HandleStructureSpawn(RequestedClass, BuildLocation, this);
	//}
}