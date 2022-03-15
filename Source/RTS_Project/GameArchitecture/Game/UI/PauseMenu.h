// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"

#include "ExternalWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UPauseMenu : public UExternalWidget
{
	GENERATED_BODY()
public:
   /** Sets the visibility of the widget. */
  virtual void SetVisibility(ESlateVisibility InVisibility) override;

protected:  
  virtual bool Initialize() override;


public:
  virtual void SetResumeHotKey(const FName ActionName);

protected:
    UFUNCTION()
    virtual void OnResumePlayButtonPressed();

    UFUNCTION()
    virtual void OnReturnToLobbyButtonPressed();

    UFUNCTION()
    virtual void OnExitToMainMenuButtonPressed();


protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ResumePlayButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ReturnToLobbyButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitToMainMenuButton;

protected:
    UPROPERTY(EditDefaultsOnly)
    FName ResumeHotKey = FName("EscapeKey");
};
