// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockPathFollowingComponent.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"


void UFlockPathFollowingComponent::OnActorBump(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	/*
	Super::OnActorBump(SelfActor, OtherActor, NormalImpulse, Hit);
	ARTSMinion * selfminion =  Cast<ARTSMinion>(SelfActor);
	ARTSMinion* otherminion = Cast<ARTSMinion>(OtherActor);
	
	if (selfminion && otherminion && (selfminion->GetTeam() == otherminion->GetTeam()))
	{
		FVector fv = selfminion->GetActorForwardVector();
		fv = fv.RotateAngleAxis(90.0f, FVector(0.0, 0.0, 1.0));
		float bumpdistance = 60.0f;

		if (otherminion->GetVelocity().IsZero())
		{
			FVector otherlocation = otherminion->GetActorLocation();
			FVector finallocation = otherlocation + (fv*bumpdistance);

			otherminion->RtsMove(finallocation);
		}
	}

	*/
}