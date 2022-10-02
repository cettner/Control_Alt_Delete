// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamResourcePanelWidget.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h" 

bool UTeamResourcePanelWidget::Initialize()
{
	bool retval = Super::Initialize();

	if (ResourcePanel != nullptr)
	{
		const UWorld* world = GetWorld();
		if (world == nullptr) return false;

		ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
		if (gs != nullptr)
		{
			ATeamResourceState* tr = gs->GetDefaultTeamState<ATeamResourceState>();
			if (tr != nullptr)
			{
				FReplicationResourceMap teamresources = tr->GetAllHeldResources();

				for (int i = 0; i < teamresources.Num(); i++)
				{
					UTeamResourceSlotWidget* resourceslot = CreateWidget<UTeamResourceSlotWidget>(this, ResourceSlotWidgetClass);
					resourceslot->Setup(teamresources[i].Key);
					ResourcePanel->AddChild(resourceslot);
				}
			}
		}
	}
	else
	{
		retval = false;
	}



	return retval;
}