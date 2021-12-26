// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DefaultPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADefaultPlayerState(const FObjectInitializer& ObjectInitializer);

public:
	/*Returns wether the first set of data has been replicated from the server after game login, this always returns true on the server.*/
	bool IsClientInitialized();
	
	/*Whether Gameode Data has been loaded through the gamestate to the local player*/
	bool HasDefaultGameModeLoaded();

	/*Allow the Local Player to Initialize Data from the Default GameMode, after its class has been recieved by the GameState*/
	virtual void LoadGameModeDefaults(const AGameModeBase * GameModeCDO);

	bool IsLocalPlayerState() const;

	void SetLocalPlayerState(bool inLocalState);

protected:
	/*Virtual Function For Override*/
	UFUNCTION()
	virtual void OnRep_TeamID();

protected:
	virtual void ClientInitialize(AController* Controller) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


protected:
	/*True if the initial data has been replicated from the server*/
	bool bisClientInitialized = false;
	bool bHasDefaultGameModeDataLoaded = false;
	bool bisLocalPlayerState = false;

public:
	UPROPERTY(ReplicatedUsing = OnRep_TeamID)
	int TeamID;
	
};
