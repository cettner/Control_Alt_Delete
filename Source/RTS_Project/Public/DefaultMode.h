// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultMode.generated.h"

/**
 * 
 */
/*Foward Declarations*/
class ARTSCamera;
class ACommander;

UCLASS()
class RTS_PROJECT_API ADefaultMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADefaultMode(const FObjectInitializer& ObjectInitializer);
	virtual void PostLogin(APlayerController * NewPlayer) override;
	virtual void Logout(AController * Exiting) override;
	int GetNumTeams() { return (num_teams); }
	int GetTeamSize() { return(team_size);  }

	/** The default pawn class used by RTS players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ARTSCamera> DefaultRTSClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ACommander> DefaultFPSClass = nullptr;

protected:
	virtual UClass * GetDefaultPawnClassForController_Implementation(AController * InController) override;

private:
	int MaxRTSPlayersPerTeam = 1;
	int num_teams = 2;
	int team_size = 2;
};
