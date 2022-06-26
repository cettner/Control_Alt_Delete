// Fill out your copyright notice in the Description page of Project Settings.


#include "TabButtonWidget.h"



void UTabButtonWidget::SetActive(const bool InActiveState)
{
	bIsActiveTab = InActiveState;
	OnTabActiveStateChange(bIsActiveTab);
}

bool UTabButtonWidget::IsActiveTab() const
{
	return bIsActiveTab;
}

FSlateColor UTabButtonWidget::GetTitleColor()
{
	FLinearColor retval;
	if (bIsActiveTab == true)
	{
		retval = ActiveTextColor;
	}
	else
	{
		retval = InActiveTextColor;
	}

	return FSlateColor(retval);
}

UButton* UTabButtonWidget::GetButton()
{
	return InnerButton;
}

void UTabButtonWidget::SetBoundWidget(UWidget* InWidget)
{
	BoundWidget = InWidget;
}

UWidget* UTabButtonWidget::GetBoundWidget() const
{
	return BoundWidget;
}

bool UTabButtonWidget::IsWidgetBound() const
{
	return IsValid(BoundWidget);
}

bool UTabButtonWidget::Initialize()
{
	bool retval = Super::Initialize();

	if (IsValid(TabNameTextBlock))
	{
		TabNameTextBlock->SetText(TabTitleName);
		TabNameTextBlock->ColorAndOpacityDelegate.BindDynamic(this, &UTabButtonWidget::GetTitleColor);
	}


	return retval;
}

void UTabButtonWidget::OnTabActiveStateChange(bool InNewState)
{

}
