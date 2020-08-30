// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "../../GameArchitecture/Instance/LobbyGameInstance.h"
#include "LobbyGameState.generated.h"



USTRUCT()
struct FSlotPlayerData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString PlayerName = "";

	UPROPERTY()
	int32 TeamId = -1;

	UPROPERTY()
	int32 SlotId = -1;

	UPROPERTY()
	bool isSlotActive = false;

	UPROPERTY()
	uint32 OwningPlayerID = 0xFFFFU;
};

USTRUCT()
struct FLobbyData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FSlotPlayerData> TeamData;

	int ActiveNum()
	{
		int activecount = 0;
		for (int i = 0; i < TeamData.Num(); i++)
		{
			if (TeamData[i].isSlotActive)
			{
				activecount++;
			}
		}
		return(activecount);
	}

	FLobbyData() { TeamData = TArray<FSlotPlayerData>();}
};



class ALobbyPlayerController;

UCLASS()
class RTS_PROJECT_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()


	public:
		ALobbyGameState();
		TArray<FLobbyData> GetLobbyData();
		bool AddPlayertoLobby(ALobbyPlayerController * NewPlayer);

		bool RemovePlayerFromLobby(ALobbyPlayerController * LeavingPlayer);

		UFUNCTION(Server, reliable, WithValidation)
		void ServerRequestMoveSlot(ALobbyPlayerController * RequestingPlayer, FSlotPlayerData RequestedSlot);

		virtual bool RequestStartGame(ALobbyPlayerController * RequestingPlayer);
		virtual bool CanPlayerStartGame(ALobbyPlayerController * Player);
		virtual bool StoreLobbyData();
		
		virtual bool IsGameStarting() const;
	
	protected:
		virtual bool StorePlayerData(ULobbyGameInstance * GI);
		virtual bool StoreServerData(ULobbyGameInstance * GI);
		virtual void SetCustomPlayerSettings(FPlayerSettings &outsettings, const FSlotPlayerData StoredSlot);



	protected:

		UPROPERTY(ReplicatedUsing = OnRep_LobbyInfo)
		TArray<FLobbyData> LobbyData;

		UFUNCTION()
		void OnRep_LobbyInfo();

		bool bisGameStarting = false;

	private:
		bool FindPlayerinLobby(ALobbyPlayerController * player ,FSlotPlayerData& OutSlot);

	private:
		uint32 PlayersinLobby = 0;
		uint32 MaxPlayers = 0;

		/*Data Copied from GameInstance for Server Use*/
		uint32 NumTeams = 0;
		uint32 NumPlayersPerTeam = 0;

protected:
		void PostInitializeComponents() override;
		void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
