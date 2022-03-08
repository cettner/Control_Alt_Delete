// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
    virtual bool Initialize() override;

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
};
