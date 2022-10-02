// Fill out your copyright notice in the Description page of Project Settings.


#include "PersonalResourceWidget.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"

void UPersonalResourceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PercentDelegate.BindUFunction(this, "UpdateWeightCapacity");
}

float UPersonalResourceWidget::UpdateWeightCapacity()
{
	APawn* MyPawn = GetOwningPlayerPawn();
	IResourceGatherer * resourcecarrier =  GetOwningPlayerPawn<IResourceGatherer>();
	float retval = 0.0f;

	if (resourcecarrier && (resourcecarrier->GetMaxWeight() > 0U))
	{
		const float currweight = (float)resourcecarrier->GetCurrentWeight();
		const float maxweight = (float)resourcecarrier->GetMaxWeight();
		retval = currweight / maxweight;
	}

	return retval;
}
