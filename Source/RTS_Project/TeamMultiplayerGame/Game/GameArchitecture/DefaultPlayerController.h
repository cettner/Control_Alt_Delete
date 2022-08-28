// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

#include "../../LobbyGameInstance.h"
#include "DefaultPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()
	

	public:
		bool GetPlayerInfo(FPlayerSettings& outsettings) const;

		bool IsRegistered() const { return(bisregistered); }

		virtual int GetTeamID() const;

		void SetIsRegistered(bool bregistered);

		/*Attempts to call FinishLocalPlayersetup after registration is complete, if called before beginplay is kicked off deferes playerstartup to beginplay for the controller*/
		void PostRegisterInit();

		/*Called By DefaultGameState After Initial Data Has Been Replicated Down*/
		virtual void ClientInitUI();

	public:
		virtual void OpenExternalMenu(UUserWidget* InMenu);
		virtual bool IsExternalMenuOpen() const;
		virtual void CloseExternalMenu();
		virtual void OnBeginPause();
		virtual void OnEndPause();

	protected:
		UFUNCTION()
		virtual void OnEscapeActionPressed();

	protected:

		/*Virtual Function that is called after the world is ready registration is complete and data on the client is initialized*/
		virtual void FinishLocalPlayerSetup();

		UFUNCTION()
		void OnRep_bisregistered();

	protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
		virtual void SetupInputComponent() override;
		virtual void InitPlayerState() override;
		virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;

	protected:
		/*Replicated After Player data is registered by the server during post login or on request*/
		UPROPERTY(ReplicatedUsing = OnRep_bisregistered)
		bool bisregistered = false;

		bool isHUDInitialized = false;

		/*set to to true when the gamestate is initally replicated down if it beats player registration*/
		bool battemptedHudinit = false;

	protected:
		UUserWidget* ExternalMenu = nullptr;


};
