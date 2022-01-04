// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceDropPoint.h"
#include "DrawDebugHelpers.h"
#include "Components/ShapeComponent.h"

#include "RTS_Project/RTSFPS/Shared/Interfaces/ResourceGatherer.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"


AResourceDropPoint::AResourceDropPoint() : Super()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AResourceDropPoint::OnOverlapBegin);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AResourceDropPoint::OnOverlapEnd);
}

void AResourceDropPoint::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (HasAuthority())
	{
		IResourceGatherer * gatherer = Cast<IResourceGatherer>(OtherActor);
		IRTSObjectInterface * rtsobject = Cast<IRTSObjectInterface>(OtherActor);
		const UWorld * world = GetWorld();
		
		if (gatherer != nullptr)
		{
			FReplicationResourceMap resources = gatherer->GetAllHeldResources();

			for (int i = 0; i < resources.Num(); i++)
			{
				ARTFPSGameState * gs = world->GetGameState<ARTFPSGameState>();
				gs->ScoreResource(resources[i].Key,resources[i].Value, rtsobject);
				gatherer->RemoveResource(resources[i].Key, resources[i].Value);
			}
		}
	}

}

void AResourceDropPoint::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}


void AResourceDropPoint::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AResourceDropPoint::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);

}
