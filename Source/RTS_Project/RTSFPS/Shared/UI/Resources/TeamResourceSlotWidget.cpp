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

	if (IsValid(ResourceNameText) && (DisplayResourceClass != nullptr))
	{
		const AResource* resourcecdo = DisplayResourceClass->GetDefaultObject<AResource>();
		const FName resourcename = resourcecdo->GetUnitName();
		ResourceNameText->Text = FText::FromName(resourcename);
	}

}

FText UTeamResourceSlotWidget::UpdateCurrentResourceValueText()
{
	FText Text = FText::FromString(FString("Error"));
	UWorld* World = GetWorld();

	if (World == nullptr) return(Text);

	ARTFPSGameState * gs = World->GetGameState<ARTFPSGameState>();
	if (gs == nullptr) return(Text);

	ATeamResourceState* tr = gs->GetDefaultTeamState<ATeamResourceState>();
	if (tr == nullptr) return(Text);

	int retval = tr->GetHeldResource(DisplayResourceClass);

	Text = FText::FromString(FString::FromInt(retval));

	return Text;
}
