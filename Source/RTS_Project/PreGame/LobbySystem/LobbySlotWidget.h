// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "LobbySlotWidget.generated.h"




UCLASS()
class RTS_PROJECT_API ULobbySlotWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		bool IsSlotFilled();

	public:
		virtual void Setup(FSlotPlayerData SlotInfo);
		virtual void ActivateSlot(FSlotPlayerData SlotInfo);
		virtual void DeactivateSlot();

	protected:
		virtual bool Initialize() override;

	private:
		bool bIsSlotFilled;

	protected:
		UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher * SlotActiveSwitcher;

		/*Empty Widget Slot*/
		UPROPERTY(meta = (BindWidget))
		UWidget * EmptySlot;

		UPROPERTY(meta = (BindWidget))
		UButton * JoinSlotButton;

		/*Filled Widget Slot*/
		UPROPERTY(meta = (BindWidget))
		UWidget * FilledSlot;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerName;

	//	UPROPERTY(meta = (BindWidget))
	//	UButton * KickButton;
};