// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_Project/LobbySystem/GameArchitecture/Instance/LobbyGameInstance.h"
#include "DefaultPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	protected:
		void PostInitializeComponents() override;

	public:
		virtual bool GetPlayerInfo(FPlayerSettings& outsettings);

		UFUNCTION(Client, reliable)
		void ClientRequestRegistration();

		bool IsRegistered() const { return(bisregistered); }

		virtual void ClientNotifyTeamChange(int newteamid);

		virtual int GetTeamID() const;

	protected:
		UFUNCTION(Server, reliable, WithValidation)
		void ServerRegisterPlayerInfo(FPlayerSettings settings);

		virtual void RegisterPlayerInfo(FPlayerSettings settings);

		virtual void RequestRegistration();

		bool bisregistered = false;

};
