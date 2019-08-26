// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultGameState.h"
#include "FogOfWarManager.h"
#include "RTFPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTFPSGameState : public ADefaultGameState
{
	GENERATED_BODY()

	public:
		ARTFPSGameState(const FObjectInitializer & FOI);
		int NumRTSPlayers(int Team_Index);
		FORCEINLINE AFogOfWarManager * GetFogOfWar() {return(FOWManager);}

	protected:
		virtual void BeginPlay() override;
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	protected:
		virtual AFogOfWarManager * InitFOW();
	
	private:
		TSubclassOf<AFogOfWarManager> FOWManagerClass;
		UPROPERTY(Replicated)
		AFogOfWarManager * FOWManager;



};
