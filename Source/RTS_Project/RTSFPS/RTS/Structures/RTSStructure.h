// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/MenuInteractableInterface.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/DeathComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceVendorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridAttachmentActor.h"
#include "Interfaces/BuildableInterface.h"

#include "NavModifierComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "RTSStructure.generated.h"

enum ERTSStructureSpawnSuccess
{
	SUCCESS,
	COLLISION_FAIL,
};


USTRUCT()
struct FStructureQueueData
{
	GENERATED_USTRUCT_BODY()
	TSubclassOf<UObject> SpawnClass = nullptr;
	AController* RecieveingController = nullptr;
	float SpawnTime = 0.0f;
};

USTRUCT()
struct FStructureSpawnData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "RTSObjectInterface"))
	TSubclassOf<UObject> SpawnClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float SpawnTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, NotReplicated)
	UTexture* MinionThumbnail = nullptr;

	UPROPERTY(EditDefaultsOnly)
	bool bIsEnabled = true;

	UPROPERTY(EditDefaultsOnly)
	FString MinionName =  "";
};

/*Forward Declaration*/
class UStructureSpawnQueueWidget;

UCLASS()
class RTS_PROJECT_API ARTSStructure : public AGridAttachmentActor, public IRTSObjectInterface, public IMenuInteractableInterface, public IBuildableInterface, public IResourceVendorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTSStructure();

	/*************RTSObject Interface Overrides******************/
public:
	virtual void SetSelected() override;
	virtual void SetDeselected() override;
	virtual int GetTeam() const override;
	virtual void SetTeam(int newteamindex) override;
	virtual TArray<URTSProperty*> GetRTSProperties(bool bIncludeNestedProperties = false) const override;
	virtual void IssueOrder(AController* InIssuer, const FHitResult& InHitContext, URTSOrder* InOrderClass = nullptr, const bool InbIsQueuedOrder = false) override;
	virtual void OnLocalPlayerTeamChange(int InLocalTeamID) override;
	virtual bool IsLocalEnemy() const override;

protected:
	virtual void RegisterRTSObject() override;
	virtual void UnRegisterRTSObject() override;
	virtual void SetTeamColors(FLinearColor TeamColor) override;
	/************************************************************/

	/**************IBuildable Interface Overrides****************/
public:
	virtual float GetPercentConstructed() const override;
	virtual bool IsConstructed() const override;
	virtual void BeginConstruction() override;
	
	UFUNCTION()
	virtual void IncrementConstruction(float DeltaConstruction, AActor * Contributor = nullptr) override;

protected:
	virtual void OnConstructionComplete() override;
	/************************************************************/

	/***********IResourceVendor Interface Overrides**************/
public:
	virtual TArray<TSubclassOf<UObject>> GetPurchasableUnitsForSource(const IResourceGatherer* Purchaser = nullptr, const AController* InstigatingController = nullptr) const override;

protected:
	virtual const TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetAllDefaultUnitPrices() const override;
	/************************************************************/
protected:
	UFUNCTION()
	virtual void OnDeath();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UFUNCTION()
	virtual void OnRep_TeamID();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	bool QueueActor(TSubclassOf<UObject> ObjectClass, AController* InheritingController = nullptr);

	virtual bool IsDropPointFor(TSubclassOf<UResource> ResourceType) const;

	bool IsQueueFull() const;

	bool CanSpawn(TSubclassOf<UObject> Objectclass) const;

	int GetIndexByClass(TSubclassOf<UObject> Objectclass) const;

	TArray< FStructureSpawnData> GetSpawnData() const;

	float GetCurrentQueueStatus() const;

	uint32 GetCurrentQueueSize() const;

	uint32 GetMaxQueueSize() const;

	bool ScoreResource(IResourceGatherer * Donar);

	FTransform FindActorSpawnLocation(FVector BoxExtent) const;

protected:

	void UpdateSpawnQueue();
	void SpawnUnit(FStructureQueueData QueueData);
	void CancelSpawn();


protected:
	/*True if the Structure does not need to be constructed upon entering the world, by default objects placed in the level before game start do not need construction*/
	UPROPERTY(EditAnywhere, Category = Construction)
	bool bSkipsConstruction = HasAnyFlags(RF_WasLoaded);

	UPROPERTY(EditAnywhere, Category = Construction, meta = (EditCondition = "!bSkipsConstruction"))
	bool bAutoBuilds = false;

	UPROPERTY(EditAnywhere, Category = Construction, meta = (EditCondition = "bAutoBuilds && !bSkipsConstruction"))
	float AutoBuildRate = 5.0f;

	UPROPERTY(EditAnywhere, Category = Construction, meta = (EditCondition = "bAutoBuilds && !bSkipsConstruction"))
	float AutoBuildPerTick = 0.10f;

	FTimerHandle BuildUpdateHandler;
	FTimerDelegate BuildUpdateDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = DeathComp)
	UDeathComponent* DeathComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = DeathComp)
	float UnConstructedHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = DeathComp)
	TArray<UAnimMontage*> DestroyAnimations = TArray<UAnimMontage*>();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Selection)
	UDecalSelectionComponent* Selection = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MeshComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Navigation)
	UNavModifierComponent* NavModifierComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Navigation)
	UBoxComponent* ResourceDropBounds = nullptr;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_TeamID, EditAnywhere, Category = Gameplay)
	int TeamID = -1;


protected:
	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	TArray<FStructureSpawnData> SpawnableUnits;

	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	TEnumAsByte<ECollisionChannel> SpawnTraceChannel;

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxQueueSize = 10;

protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UStructureSpawnQueueWidget> MenuClass;

	/***********************Runtime DATA****************************/

protected:
	/*SPAWN DATA*/
	FTimerHandle QueueHandler;

	uint32 CurrentQueueSize = 0;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float queuestatus = 0.0f;

	TQueue<FStructureQueueData> StructureQueue;

protected:
	bool bIsLocalEnemy = false;

protected:
	UStructureSpawnQueueWidget* Menu;


#if WITH_EDITOR
	/*need this because for some reason editor world is obtained sometimes instead of PIE from GetWorld() call*/
	UWorld * GetWorldPIE() const;
#endif
};
