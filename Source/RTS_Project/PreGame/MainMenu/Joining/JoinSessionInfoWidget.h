// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "RTS_Project/LobbySystem/Interfaces/SessionMenuInterface.h"
#include "JoinSessionInfoWidget.generated.h"

/**
 * 
 */


UCLASS()
class RTS_PROJECT_API UJoinSessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()



public:
	void Setup(const FOnlineSessionSearchResult InSearchResult, int32 InIndex);
	virtual void SetSelected(bool bIsSelected);
	int32 GetIndex() const;
	virtual bool IsPrivateGame(const FOnlineSessionSettings  InSettings) const;

protected:
	virtual bool Initialize() override;

protected:
	UFUNCTION()
	void OnSelectServerButtonPressed();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerNameTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HostUserNameTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ConnectionFractionTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* GamePrivacyTypeTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PingTextBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectServerButton;

protected:
	int32 Index = -1;

	bool bISelected = false;
	
};
