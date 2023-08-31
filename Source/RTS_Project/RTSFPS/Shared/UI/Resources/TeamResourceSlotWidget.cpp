// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamResourceSlotWidget.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h" 

void UTeamResourceSlotWidget::Setup(TSubclassOf<AResource> InResourceClass)
{
	DisplayResourceClass = InResourceClass;

	if (IsValid(ResourceValueText) && (DisplayResourceClass != nullptr))
	{
		ResourceValueText->TextDelegate.BindUFunction(this, "UpdateCurrentResourceValueText");
	}
	
	if (DisplayResourceClass != nullptr)
	{
		const AResource* resourcecdo = DisplayResourceClass->GetDefaultObject<AResource>();
		ResourceName = resourcecdo->GetUnitName();
	}

}

FText UTeamResourceSlotWidget::UpdateCurrentResourceValueText()
{
	FText retval = FText::FromString(FString("Error"));
	UWorld* World = GetWorld();

	if (World == nullptr) return(retval);

	ARTFPSGameState * gs = World->GetGameState<ARTFPSGameState>();
	if (gs == nullptr) return(retval);

	ATeamResourceState* tr = gs->GetDefaultTeamState<ATeamResourceState>();
	if (tr == nullptr) return(retval);

	const int resourceval = tr->GetHeldResource(DisplayResourceClass);


	
	if (bIncludeResourceName == true)
	{
		retval = FText::FromString(ResourceName.ToString() + " : " + FString::FromInt(resourceval));
	}
	else
	{
		retval = FText::FromString(FString::FromInt(resourceval));
	}

	return retval;
}
