// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultPlayerState.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTFPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTFPSPlayerState : public ADefaultPlayerState
{
	GENERATED_BODY()
	
public:
	bool IsRTSPlayer() const;
	void SetIsRTSPlayer(bool isrts);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated)
	bool isRtsPlayer = false;	
};
