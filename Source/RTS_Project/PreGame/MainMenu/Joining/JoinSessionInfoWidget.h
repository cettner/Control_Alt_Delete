// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "JoinSessionInfoWidget.generated.h"

/**
 * 
 */

/*Foward Declarations*/
class UMainMenu;

UCLASS()
class RTS_PROJECT_API UJoinSessionInfoWidget : public UUserWidget
{
	GENERATED_BODY()
		

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostUser;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionFraction;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(UMainMenu * InParent, uint32 InIndex);

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

	UPROPERTY()
	UMainMenu * Parent;

	uint32 Index = -1;

	UFUNCTION()
	void OnClicked();
	
};
