// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Info.h"
#include "DefaultMode.h"
#include "DefaultPlayerState.h"
#include "TeamState.generated.h"

UCLASS()
class RTS_PROJECT_API ATeamState : public AInfo
{
	GENERATED_BODY()

	friend class ADefaultGameState;

	protected:
		ATeamState();

	public:
		virtual bool AddPlayer(ADefaultPlayerState * InPlayerstate);
		virtual bool RemovePlayer(ADefaultPlayerState* InPlayerstate);

	public:
		virtual int GetTeam() const;
		virtual bool IsPlayerOnTeam(ADefaultPlayerState* InPLayerState) const;
		virtual int32 GetCurrentPlayerNum() const;
		virtual void SetTeam(const int InTeamID);

	protected:
		virtual void LoadServerDefaults(ADefaultMode* InGameMode);

	protected:
		UFUNCTION()
		virtual void OnRep_TeamID();

	protected:
		virtual void OnRep_Owner() override;

	protected:
		virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	protected:
		UPROPERTY(ReplicatedUsing = OnRep_TeamID)
		int TeamID = -1;

		TArray<ADefaultPlayerState*> Players = TArray<ADefaultPlayerState*>();
};
