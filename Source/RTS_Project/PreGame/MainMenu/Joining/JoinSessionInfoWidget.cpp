// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinSessionInfoWidget.h"
#include "PreGame/MainMenu/MainMenu.h"

void UJoinSessionInfoWidget::Setup(UMainMenu * InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UJoinSessionInfoWidget::OnClicked);
}

void UJoinSessionInfoWidget::OnClicked()
{
	if (Parent == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[USessionRow::OnClicked] Index %i"), Index);

	Parent->SelectIndexSessionList(Index);
}
