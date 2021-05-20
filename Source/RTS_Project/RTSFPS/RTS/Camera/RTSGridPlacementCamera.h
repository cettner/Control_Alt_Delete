// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSSelectionCamera.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridAttatchmentActor.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"


#include "RTSGridPlacementCamera.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSGridPlacementCamera : public ARTSSelectionCamera
{
	GENERATED_BODY()

	public:
		AGridAttatchmentActor * CreatePlacementActor(const TSubclassOf<AActor> InActorClass) const;
		AGridAttatchmentActor * GetPlacementActor() const;
		bool IsPlacingActor() const;
		ASquareGameGrid * GetCurrentGrid() const;

	protected:
		UActorComponent* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass) const;

    protected:

		virtual void Tick(float DeltaTime) override;
		virtual void PostInitializeComponents() override;


    protected:
		/*Class of Grid Actor Used, Spawned and then set to */
		UPROPERTY(EditDefaultsOnly, Category = Grid)
		TSubclassOf<AGridAttatchmentActor> GridActorClass;

		AGridAttatchmentActor * PlacementActor = nullptr;

	protected:
		/*TEMPSTUFF*/
		UPROPERTY(EditDefaultsOnly, Category = Grid)
		TSubclassOf<ARTSStructure> StructureClass;
};
