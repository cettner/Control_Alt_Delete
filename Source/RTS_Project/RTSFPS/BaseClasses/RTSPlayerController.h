// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTS_Project/RTSFPS/RTS/FOW/FogOfWarManager.h"
#include "RTS_Project/GameArchitecture/Game/DefaultPlayerController.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSPlayerState.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */


UCLASS()
class RTS_PROJECT_API ARTSPlayerController : public ADefaultPlayerController
{
	GENERATED_BODY()
	
public:
	ARTSPlayerController();

	ARTSHUD * HudPtr;


/**************************LOCAL DATA**************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> SelectedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSStructure*> SelectedStructures;
/*************************************************************************/


	UFUNCTION(Server, reliable, WithValidation)
	void MoveMinions(ARTSPlayerController * PC, const TArray<ARTSMinion *>& Units, FHitResult Hit);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerPurchaseMinion(ARTSStructure * SpawningStructure, TSubclassOf<AActor> RequestedClass);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerPurchaseStructure(TSubclassOf<AActor> RequestedClass, FTransform BuildLocation);

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void ClientNotifyTeamChange(int newteam) override;

	virtual void FinishLocalPlayerSetup() override;
	
	virtual AFogOfWarManager * InitFOW();


public: 
	virtual void InitHUD();

public:
	virtual void OpenExternalMenu(UUserWidget * InMenu);
	virtual void CloseExternalMenu();
	UUserWidget* ExternalMenu;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fog Of War")
	TSubclassOf<AFogOfWarManager> FOWManagerClass;
	AFogOfWarManager * FOWManager;

};
