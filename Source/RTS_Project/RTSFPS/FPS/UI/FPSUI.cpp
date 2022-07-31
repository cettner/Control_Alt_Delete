// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSUI.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h" 




UFPSUI::UFPSUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TeamResourceList = CreateDefaultSubobject<UPanelWidget>(TEXT("Team Resource List"));

	ResourceWidgetClass = nullptr;
}

bool UFPSUI::Initialize()
{
	bool success = Super::Initialize();

	if (TeamResourceList && (ResourceWidgetClass != nullptr))
	{
		UWorld* World = GetWorld();
		ARTFPSGameState* GS = World->GetGameState<ARTFPSGameState>();
		if (GS == nullptr) return false;

		TArray<FResourceUIData> ResourceInfo = GS->GetMapResourceInfo();
		for (int i = 0; i < ResourceInfo.Num(); i++)
		{
			UTeamResourceWidget * child = CreateWidget<UTeamResourceWidget>(World, ResourceWidgetClass);
			child->Setup(ResourceInfo[i]);
			TeamResourceList->AddChild(child);
		}
	}
	else
	{
		success = false;
	}

	return success;
}