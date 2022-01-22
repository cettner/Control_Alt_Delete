// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/MenuInteractableInterface.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "RTS_Project/RTSFPS/GameObjects/Resource.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridClaimingActor.h"
#include "Interfaces/BuildableInterface.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/SkeletalMeshActor.h"
#include "RTS_Project/AssetHelpers/GameAssets.h"
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
class RTS_PROJECT_API ARTSStructure : public AGridClaimingActor, public IRTSObjectInterface, public IMenuInteractableInterface, public IBuildableInterface
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
	virtual void SetTeamColors(FLinearColor TeamColor) override;
	/************************************************************/

	/***********IMenuInteractable Interface overrides************/
public:
	virtual UUserWidget* GetMenu() override;
	virtual bool CanOpenMenu(APawn * InvokingPawn) const override;

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
protected:
	UFUNCTION()
	virtual void OnDeath();

protected:
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:
	UFUNCTION()
	virtual void OnRep_TeamIndex();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	bool QueueActor(TSubclassOf<UObject> ObjectClass, AController* InheritingController = nullptr);

	virtual bool IsDropPointFor(TSubclassOf<AResource> ResourceType) const;

	bool IsQueueFull() const;

	bool CanSpawn(TSubclassOf<UObject> Objectclass) const;

	int GetIndexByClass(TSubclassOf<UObject> Objectclass) const;

	TArray< FStructureSpawnData> GetSpawnData() const;

	float GetCurrentQueueStatus() const;

	uint32 GetCurrentQueueSize() const;

	uint32 GetMaxQueueSize() const;

	bool ScoreResource(TSubclassOf<AResource> ResourceType, int Amount, AActor* Donar = nullptr);

	FTransform FindActorSpawnLocation(FVector BoxExtent) const;

protected:

	void UpdateSpawnQueue();
	void SpawnUnit(FStructureQueueData QueueData);
	void CancelSpawn();


protected:
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	UHealthComponent* Health;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float UnConstructedHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	TArray<UAnimMontage*> DestroyAnimations = TArray<UAnimMontage*>();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Selection)
	UDecalSelectionComponent* Selection;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MeshComp;

protected:

	UPROPERTY(EditDefaultsOnly, Replicated, Category = Gameplay)
	int TeamIndex = -1;

protected:
	/*SPAWN DATA*/
	FTimerHandle QueueHandler;

	uint32 CurrentQueueSize = 0;

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxQueueSize = 10;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float queuestatus = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	TArray< FStructureSpawnData> SpawnableUnits;

	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	TEnumAsByte<ECollisionChannel> SpawnTraceChannel;

	TQueue<FStructureQueueData> StructureQueue;


protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UStructureSpawnQueueWidget> MenuClass;

	UStructureSpawnQueueWidget* Menu;



#if WITH_EDITOR
	/*need this because for some reason editor world is obtained sometimes instead of PIE from GetWorld() call*/
	UWorld * GetWorldPIE() const;
#endif
};
