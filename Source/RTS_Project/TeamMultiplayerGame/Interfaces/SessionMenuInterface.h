// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnlineSessionSettings.h"
#include "SessionMenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USessionMenuInterface : public UInterface
{
	GENERATED_BODY()
};

enum ESessionPrivacyType
{
	Public,
	FriendsOnly,
	Private
};

class RTS_PROJECT_API ISessionMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Host(const FString InServerName, const FSessionSettings CustomSettings = FSessionSettings()) = 0;

	virtual void JoinSession(uint32 Index) = 0;

	virtual void EndSession() = 0;

	virtual void BeginSearchQuery() = 0;

	virtual bool IsSearchingSession(const FName SessionName = "") = 0;
	//virtual bool IsHosting() const = 0;

};
