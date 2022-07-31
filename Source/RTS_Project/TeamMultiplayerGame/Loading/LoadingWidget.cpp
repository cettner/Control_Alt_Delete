#include "LoadingWidget.h"
#include "..\LobbyGameInstance.h"


void ULoadingWidget::SetTransitionURL(FURL InURL, UGameInstance* InInstance)
{
	DestinationURL = InURL;
	UGameInstance* gi;
	
	if (InInstance == nullptr)
	{
		/*Note, this call will fail during initial game startup*/
		gi = GetGameInstance();
	}
	else
	{
		gi = InInstance;
	}

	if (gi != nullptr)
	{
		/*Set a Timer for the minimum displayed time to load*/
		FTimerManager& timer = gi->GetTimerManager();
		timer.SetTimer(MinLoadTimeHandle, this, &ULoadingWidget::OnMinLoadTimeElapsed, MinDisplayTime, false);
		
		/*Attatch the Widget to the Viewport client so it perists between level loads*/
		UGameViewportClient* viewport = gi->GetGameViewportClient();
		checkf(viewport, TEXT("ULoadingWidget::Initialize ViewPort was Null"));
		viewport->RemoveAllViewportWidgets();
		viewport->AddViewportWidgetContent(TakeWidget(), 1000);

		if (ShowWindowAnimation != nullptr)
		{
			PlayAnimation(ShowWindowAnimation);
		}
		else if (ShowTipAnimation != nullptr)
		{
			PlayAnimation(ShowTipAnimation);
		}
	}


}


bool ULoadingWidget::Initialize()
{
	const bool retval = Super::Initialize();
	


	return retval;
}

void ULoadingWidget::OnMinLoadTimeElapsed()
{
	bMinLoadTimeElapsed = true;
	if (!IsLoading())
	{
		OnTotalLoadComplete();
	}
}

bool ULoadingWidget::IsLoading() const
{
	const bool retval = !bMinLoadTimeElapsed || !bIsLevelLoaded;

	return retval;
}

void ULoadingWidget::OnLevelLoadComplete(bool InForceClose)
{
	bIsLevelLoaded = true;
	if (!IsLoading() || InForceClose)
	{
		OnTotalLoadComplete();
	}
}

void ULoadingWidget::OnTotalLoadComplete()
{

	if (HideWindowAnimation != nullptr && !bIsEndingLoad)
	{
		bIsEndingLoad = true;
		PlayAnimation(HideWindowAnimation);
	}
	else
	{
		ULobbyGameInstance* gi = GetGameInstance<ULobbyGameInstance>();
		gi->OnLoadLevelComplete(DestinationURL, true);
	}
}

void ULoadingWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	checkf(Animation, TEXT("ULoadingWidget::OnAnimationFinished_Implementation Completed Animation was null"))
	if (Animation == ShowWindowAnimation && ShowTipAnimation != nullptr)
	{
		PlayAnimation(ShowTipAnimation);
	}
	else if (Animation == HideWindowAnimation)
	{
		OnTotalLoadComplete();
	}
}

