// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinSessionInfoWidget.h"
#include "RTS_Project/PreGame/MainMenu/MainMenu.h"


void UJoinSessionInfoWidget::Setup(FOnlineSessionSearchResult InSearchResult, int32 InIndex)
{

}

void UJoinSessionInfoWidget::SetSelected(bool bIsSelected)
{
}

int32 UJoinSessionInfoWidget::GetIndex() const
{
	return Index;
}

bool UJoinSessionInfoWidget::Initialize()
{
	bool retval = Super::Initialize();
	return retval;
}

void UJoinSessionInfoWidget::OnSelectServerButtonPressed()
{

}
