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
		virtual void PostInitializeComponents() override;
		virtual void BeginPlay() override;

	public:
		bool GetPlayerInfo(FPlayerSettings& outsettings);

		UFUNCTION(Client, reliable)
		void ClientRequestRegistration();

		bool IsRegistered() const { return(bisregistered); }

		virtual void ClientNotifyTeamChange(int newteamid);

		virtual int GetTeamID() const;

		virtual void OnMatchStart();

		virtual void SetIsRegistered(bool bregistered);

		/*Virtual Function that is called after the world is ready registration is complete and data on the client is initialized*/
		virtual void FinishLocalPlayerSetup();

	protected:
		UFUNCTION(Server, reliable, WithValidation)
		void ServerRegisterPlayerInfo(FPlayerSettings settings);

		virtual void RegisterPlayerInfo(FPlayerSettings settings);


		virtual void RequestRegistration();

		/*Attempts to call FinishLocalPlayersetup after registration is complete, if called before beginplay is kicked off deferes playerstartup to beginplay for the controller*/
		void PostRegisterInit();

		UFUNCTION()
		virtual void OnRep_bisregistered();

	protected:
		/*Replicated After Player data is registered by the server during post login or on request*/
		UPROPERTY(ReplicatedUsing = OnRep_bisregistered)
		bool bisregistered = false;



	protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	private:
		/*If Registration happens before begin play, we have to wait to finish local playersetup*/
		bool bdelaytillbeginplay = false;

};
