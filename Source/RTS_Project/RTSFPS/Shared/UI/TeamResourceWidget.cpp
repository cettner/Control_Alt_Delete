// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamResourceWidget.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h" 

void UTeamResourceWidget::Setup(FResourceUIData UIData)
{
	SetupData = UIData;

	if (ResourceValueText)
	{
		ResourceValueText->TextDelegate.BindUFunction(this, "UpdateCurrentResourceValueText");
	}

	if (ResourceNameText)
	{
		ResourceNameText->Text = FText::FromName(UIData.ResourceName);
	}

}

FText UTeamResourceWidget::UpdateCurrentResourceValueText()
{
	FText Text = FText::FromString(FString("Error"));
	UWorld* World = GetWorld();

	if (World == nullptr) return(Text);

	ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();
	if (GS == nullptr) return(Text);

	ADefaultPlayerState * PS = GetOwningPlayerState<ADefaultPlayerState>();
	if (PS == nullptr) return(Text);

	int retval = GS->GetTeamResourceValue(PS->TeamID, SetupData.Key);

	Text = FText::FromString(FString::FromInt(retval));

	return Text;
}
