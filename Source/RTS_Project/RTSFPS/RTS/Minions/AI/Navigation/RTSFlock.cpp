// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFlock.h"

void URTSFlock::UpdateFlock()
{
	for (int i = 0; i < FlockAgents.Num(); i++)
	{
		IRTSFlockAgentInterface* agent = FlockAgents[i];

		agent->CalcSeparation();
		agent->CalcAlignment();
		agent->CalcCohesion();

		// Apply obstacle avoidance
		//agent->ApplyObstacleAvoidance();

	}
}
