// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/AssetHelpers/RTSSpawnHelper.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSSelectable.h"
#include "RTSStructure.generated.h"

UCLASS()
class RTS_PROJECT_API ARTSStructure : public ARTSSelectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTSStructure(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void Update_Queue_UI_Status(float status);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	float CurrentIntegrity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	float MaxIntegrity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	bool bIsConstructed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	int teamindex;

	/*SPAWN DATA*/
	TQueue<int> SpawnQueue;

	UPROPERTY(BlueprintReadOnly)
	float queuestatus = 0.0;

	UFUNCTION(BlueprintCallable, Category = UI)
	void Queue_Minion(int minion_index);
	FTimerHandle Queue_Handler;

	const float spawndistance = 300.0;
	FVector BannerLocation;
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isdroppoint = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool Can_Spawn_Builder = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Builder_Spawn_Time = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool Can_Spawn_Catapult = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Catapult_Spawn_Time = 20.0;

public:
	int GetTeam() const;

	bool IsDropPoint() const;

private:
	RTSSpawnHelper * SHelper;

	void UpdateSpawnQueue();
	void SpawnUnit(int unit_index);
	void CancelSpawn();
	float GetSpawnTimeByIndex(Unit_Types type);
};
