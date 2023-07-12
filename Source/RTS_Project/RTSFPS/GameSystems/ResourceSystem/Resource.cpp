// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"






AResource::AResource() : Super() 
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCanEverAffectNavigation(true);
	MeshComp->bFillCollisionUnderneathForNavmesh = true;
	MeshComp->bReceivesDecals = false;
	MeshComp->SetupAttachment(RootComponent);

	SelectionComp = CreateDefaultSubobject<UDecalSelectionComponent>(TEXT("SelectionComp"));
	SelectionComp->SetDetection(MeshComp);
	SelectionComp->SetupAttachment(MeshComp);
	SetDeselected();

}

uint32 AResource::Mine(uint32 AmountToMine)
{
	if ((ResourceVal > AmountToMine) || bisinfinite)
	{
		if (!bisinfinite)
		{
			ResourceVal -= AmountToMine;
		}

		return(AmountToMine);
	}
	else // theres not enough to give so give what's left.
	{
		uint32 retval = ResourceVal;
		ResourceVal = 0;
		OnResourcesDepleted();

		return(retval);
	}
}

uint32 AResource::GetResourceWeight() const
{
	return ResourceWeight;
}

bool AResource::CanAfford(FReplicationResourceMap BuyerResources, FReplicationResourceMap SellerCost)
{
	bool retval = true;
	const TMap<TSubclassOf<AResource>, int> buyermap = BuyerResources.GetMap();

	for (TPair<TSubclassOf<AResource>, int> Elem : SellerCost.GetMap())
	{
		const int32 * outval = buyermap.Find(Elem.Key);
		if (outval != nullptr && (retval == true))
		{
			/*Buyer has the resource, now check they have enough*/
			retval &= (*outval >= Elem.Value);
		}
		else
		{
			/*Buyer did not have the needed Resource or did not have enough*/
			retval = false;
			break;
		}
	}

	return retval;
}

void AResource::SetSelected()
{
	SelectionComp->SetHiddenInGame(false);
}

void AResource::SetDeselected()
{
	SelectionComp->SetHiddenInGame(true);
}

void AResource::RegisterRTSObject()
{
	const UWorld* world = GetWorld();
	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	/*This can miss during game startup for actors placed on the map it's corrected in ARTSFPSGamestate::RefreshAllUnits()*/
	if (IsValid(gs))
	{
		gs->RegisterRTSObject(this);
	}

}

void AResource::UnRegisterRTSObject()
{
	const UWorld* world = GetWorld();
	/*During Game Shutdown this can occur*/
	if (!IsValid(world)) return;
	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	if (IsValid(gs))
	{
		gs->UnRegisterRTSObject(this);
	}
}

UTexture* AResource::GetThumbnail(const UUserWidget* InDisplayContext) const
{
	return Thumbnail;
}

FName AResource::GetUnitName() const
{
	return ResourceName;
}

int AResource::GetTeam() const
{
	return NEUTRAL_TEAM_INDEX;
}

void AResource::OnResourcesDepleted()
{

	if (OnResourceDestroyedDelegate.IsBound())
	{
		OnResourceDestroyedDelegate.Broadcast();
	}

	Destroy();
}

void AResource::BeginPlay()
{
	Super::BeginPlay();
	RegisterRTSObject();
}

void AResource::BeginDestroy()
{
	Super::BeginDestroy();
	UnRegisterRTSObject();
}

void AResource::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
		if (PerceptionSystem)
		{
			PerceptionSystem->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
		}
	}
}



