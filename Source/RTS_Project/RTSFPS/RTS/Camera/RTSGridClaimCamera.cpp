// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGridClaimCamera.h"

#include "Components/BoxComponent.h"


void ARTSGridClaimCamera::PreInitializeGridActor(AGridAttachmentActor* GridActor, const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform) const
{
}


void ARTSGridClaimCamera::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
}
