#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include"LoadingWidget.generated.h"

DECLARE_DELEGATE(FOnMinLoadTimeDelegate);

UCLASS()
class RTS_PROJECT_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	float GetMinDisplayTime() const { return MinDisplayTime; }
	bool HasMinLoadTimeElapsed() const { return bMinLoadTimeElapsed; }
	
	/*Returns whether the load menu is still needed / waiting for completion*/
	virtual bool IsLoading() const;

	virtual void SetTransitionURL(FURL InURL, UGameInstance * InInstance = nullptr);

	/*Called by Game Instance when the level is actually loaded, optional input to not wait for full delay if it loads faster than MinimumDisplayTime*/
	void OnLevelLoadComplete(bool InForceClose = false);


protected:
	UFUNCTION()
	virtual void OnMinLoadTimeElapsed();

	virtual void OnTotalLoadComplete();

public:
	FOnMinLoadTimeDelegate LoadtimeCompleteDelegate;

protected:
	virtual bool Initialize() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

protected:
	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* ShowWindowAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* HideWindowAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	UWidgetAnimation* ShowTipAnimation;

protected:
	UPROPERTY(EditDefaultsOnly)
	float MinDisplayTime = 3.0f;

protected:
	FURL DestinationURL = FURL();
	FTimerHandle MinLoadTimeHandle = FTimerHandle();
	bool bMinLoadTimeElapsed = false;
	bool bIsLevelLoaded = false;
	bool bIsEndingLoad = false;
};