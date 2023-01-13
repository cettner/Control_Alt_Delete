// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFlock.h"

void URTSFlock::UpdateFlock()
{
	for (int i = 0; i < FlockAgents.Num(); i++)
	{
		IRTSFlockAgentInterface* agent = FlockAgents[i];

		agent->ApplySeparation();
		agent->ApplyAlignment();
		agent->ApplyCohesion();

		// Apply obstacle avoidance
		agent->ApplyObstacleAvoidance();

	}
}
