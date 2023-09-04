// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamResourceSlotWidget.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h" 

void UTeamResourceSlotWidget::Setup(TSubclassOf<UResource> InResourceClass)
{
	DisplayResourceClass = InResourceClass;

	if (IsValid(ResourceValueText) && (DisplayResourceClass != nullptr))
	{
		ResourceValueText->TextDelegate.BindUFunction(this, "UpdateCurrentResourceValueText");
	}
	
	if (DisplayResourceClass != nullptr)
	{
		const UResource* resourcecdo = DisplayResourceClass->GetDefaultObject<UResource>();
		ResourceName = resourcecdo->GetResourceName();
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

	uint32 outvalue = 0U;
	checkf(tr->GetHeldResource(DisplayResourceClass, outvalue), TEXT("UTeamResourceSlotWidget::UpdateCurrentResourceValueText, Failed to obtain Resource Type"));
	
	if (bIncludeResourceName == true)
	{
		retval = FText::FromString(ResourceName.ToString() + " : " + FString::FromInt(outvalue));
	}
	else
	{
		retval = FText::FromString(FString::FromInt(outvalue));
	}

	return retval;
}
