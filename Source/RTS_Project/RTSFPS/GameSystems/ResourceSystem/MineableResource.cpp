// Fill out your copyright notice in the Description page of Project Settings.

#include "MineableResource.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "NavAreas/NavArea_Obstacle.h"





AMineableResource::AMineableResource() : Super()
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

uint32 AMineableResource::Mine(uint32 AmountToMine)
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

void AMineableResource::SetSelected()
{
	SelectionComp->SetHiddenInGame(false);
}

void AMineableResource::SetDeselected()
{
	SelectionComp->SetHiddenInGame(true);
}

FVector AMineableResource::GetUnitLocation() const
{
	return GetActorLocation();
}

void AMineableResource::RegisterRTSObject()
{
	const UWorld* world = GetWorld();
	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	/*This can miss during game startup for actors placed on the map it's corrected in ARTSFPSGamestate::RefreshAllUnits()*/
	if (IsValid(gs))
	{
		gs->RegisterRTSObject(this);
	}

}

void AMineableResource::UnRegisterRTSObject()
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

UTexture* AMineableResource::GetThumbnail(const UUserWidget* InDisplayContext) const
{
	return Thumbnail;
}

FName AMineableResource::GetUnitName() const
{
	const UResource * resourcecdo = ResourceClass.GetDefaultObject();
	return resourcecdo->GetResourceName();
}

int AMineableResource::GetTeam() const
{
	return NEUTRAL_TEAM_INDEX;
}

void AMineableResource::OnResourcesDepleted()
{

	if (OnResourceDestroyedDelegate.IsBound())
	{
		OnResourceDestroyedDelegate.Broadcast();
	}

	Destroy();
}

TSubclassOf<UResource> AMineableResource::GetResourceClass() const
{
	return ResourceClass;
}

void AMineableResource::BeginPlay()
{
	Super::BeginPlay();
	RegisterRTSObject();
}

void AMineableResource::BeginDestroy()
{
	Super::BeginDestroy();
	UnRegisterRTSObject();
}

void AMineableResource::PostInitializeComponents()
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
