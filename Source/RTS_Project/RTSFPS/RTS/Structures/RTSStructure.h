// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/MenuInteractableInterface.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSSelectionComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "RTS_Project/AssetHelpers/GameAssets.h"
#include "RTSStructure.generated.h"


USTRUCT()
struct FStructureQueueData
{
	GENERATED_USTRUCT_BODY()
	TSubclassOf<ARTSMinion> SpawnClass = nullptr;
	AController* RecieveingController = nullptr;
	float SpawnTime = 0.0f;
};


USTRUCT()
struct FStructureSpawnData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARTSMinion> MinionClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float SpawnTime = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	UTexture* MinionThumbnail = nullptr;

	UPROPERTY(EditDefaultsOnly)
	bool bIsEnabled = true;

	UPROPERTY(EditDefaultsOnly)
	FString MinionName =  "";
};

/*Forward Declaration*/
class UStructureSpawnQueueWidget;

UCLASS()
class RTS_PROJECT_API ARTSStructure : public ASkeletalMeshActor, public IRTSObjectInterface, public IMenuInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTSStructure(const FObjectInitializer& ObjectInitializer);

public:
	/*RTSObject Interface Overrides*/
	virtual void SetSelected() override;
	virtual void SetDeselected() override;
	virtual int GetTeam() const override;
	virtual void SetTeam(int newteamindex) override;

public:
    /*IMenuInteractable Interface overrides*/
	virtual UUserWidget* GetMenu() override;
	virtual bool CanOpenMenu(APawn * InvokingPawn) const override;

protected:
	virtual void PostInitializeComponents() override;

protected:
	URTSSelectionComponent * Selection;

protected:

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	bool bIsConstructed = false;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	int teamindex = -1;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	bool isdroppoint = true;

protected:
	/*SPAWN DATA*/
	FTimerHandle QueueHandler;

	uint32 CurrentQueueSize = 0;

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxQueueSize = 10;

	UPROPERTY(BlueprintReadOnly)
	float queuestatus = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	TArray< FStructureSpawnData> SpawnableUnits;

	TQueue<FStructureQueueData> StructureQueue;


protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UStructureSpawnQueueWidget> MenuClass;

	UStructureSpawnQueueWidget * Menu;

public:

	bool QueueMinion(TSubclassOf<ARTSMinion> minionclass, AFPSServerController* InheritingController = nullptr);

	bool IsDropPoint() const;

	bool IsQueueFull() const;

	bool CanSpawn(TSubclassOf<ARTSMinion> minionclass) const;

	int GetIndexByClass(TSubclassOf<ARTSMinion> minionclass) const;

	TArray< FStructureSpawnData> GetSpawnData() const;

	float GetCurrentQueueStatus() const;

	uint32 GetCurrentQueueSize() const;

	uint32 GetMaxQueueSize() const;

protected:

	void UpdateSpawnQueue();
	void SpawnUnit(FStructureQueueData QueueData);
	void CancelSpawn();
};
