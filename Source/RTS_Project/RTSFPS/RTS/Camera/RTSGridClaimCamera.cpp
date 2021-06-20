// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGridClaimCamera.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridClaimingActor.h"

#include "Components/BoxComponent.h"


void ARTSGridClaimCamera::PreInitializeGridActor(AGridAttatchmentActor* GridActor, const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform) const
{
	AGridClaimingActor * claimactor = Cast<AGridClaimingActor>(GridActor);
	if (claimactor == nullptr) return;

	UBoxComponent * defaultbox = Cast<UBoxComponent>(FindDefaultComponentByClass(InActorClass, UBoxComponent::StaticClass()));

	if (defaultbox == nullptr) return;

	FVector defaultextent = defaultbox->GetScaledBoxExtent();

	claimactor->SetBoxExtent(defaultextent);
}


void ARTSGridClaimCamera::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
}
