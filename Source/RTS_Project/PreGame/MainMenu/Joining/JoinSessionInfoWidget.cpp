// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinSessionInfoWidget.h"
#include "RTS_Project/PreGame/MainMenu/MainMenu.h"


void UJoinSessionInfoWidget::Setup(const FOnlineSessionSearchResult InSearchResult, int32 InIndex)
{
	const FName ServerNameKey = "ServerName";
	/*Required Binding*/
	FString outstring = "NULL";
	InSearchResult.Session.SessionSettings.Get(ServerNameKey, outstring);
	ServerNameTextBlock->SetText(FText::FromString(outstring));

	const bool bisprivategame = IsPrivateGame(InSearchResult.Session.SessionSettings);
	if (IsValid(GamePrivacyTypeTextBlock))
	{
		FText privacytext;
		if (bisprivategame == true)
		{
			privacytext = FText::FromString("Private");
		}
		else
		{
			privacytext = FText::FromString("Public");
		}

		GamePrivacyTypeTextBlock->SetText(privacytext);
	}


	if (IsValid(ConnectionFractionTextBlock))
	{
		const uint32 maxplayers = InSearchResult.Session.SessionSettings.NumPublicConnections;
		const uint32 currentplayers = maxplayers - InSearchResult.Session.NumOpenPublicConnections;
		
		const FString fractionstring = FString::Printf(TEXT("%d/%d"), currentplayers, maxplayers);
		const FText fractiontext = FText::FromString(fractionstring);

		ConnectionFractionTextBlock->SetText(fractiontext);
	}

	if (IsValid(PingTextBlock))
	{
		const FText pingtext = FText::FromString(FString::FromInt(InSearchResult.PingInMs));
		PingTextBlock->SetText(pingtext);
	}
}

void UJoinSessionInfoWidget::SetSelected(bool bIsSelected)
{
}

int32 UJoinSessionInfoWidget::GetIndex() const
{
	return Index;
}

bool UJoinSessionInfoWidget::IsPrivateGame(const FOnlineSessionSettings InSettings) const
{
	bool retval = false;
	const FName IsPrivateGameKey = "IsPasswordProtected";

	if (InSettings.Get(IsPrivateGameKey, retval))
	{

	}
	else
	{
		retval = false;
	}

	retval |= (InSettings.NumPrivateConnections > 0);

	return retval;
}

bool UJoinSessionInfoWidget::Initialize()
{
	bool retval = Super::Initialize();
	return retval;
}

void UJoinSessionInfoWidget::OnSelectServerButtonPressed()
{

}
