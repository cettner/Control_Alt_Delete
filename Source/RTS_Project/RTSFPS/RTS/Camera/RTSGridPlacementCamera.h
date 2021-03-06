// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSSelectionCamera.h"
#include "RTS_Project/RTSFPS/RTS/Structures/PlacementActor.h"
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
		APlacementActor * CreatePlacementActor(const TSubclassOf<AActor> InActorClass,FTransform SpawnTransform = FTransform()) const;
		APlacementActor * GetPlacementActor() const;

		template < class T >
		T* GetPlacementActor() const
		{
			return Cast<T>(GetPlacementActor());
		}

		bool IsPlacingActor() const;
		ASquareGameGrid * GetCurrentGrid() const;
		
		virtual void ToggleBuildGrid();

	protected:
		virtual void PreInitializeGridActor(AGridAttatchmentActor* GridActor, const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform = FTransform()) const;

    protected:

		virtual void Tick(float DeltaTime) override;
		virtual void PostInitializeComponents() override;
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    protected:
		/*Class of Grid Actor Used, Spawned and then set to */
		UPROPERTY(EditDefaultsOnly, Category = Grid)
		TSubclassOf<APlacementActor> PlacementActorClass;

		APlacementActor * PlacementActor = nullptr;

	protected:
		/*TEMPSTUFF*/
		UPROPERTY(EditDefaultsOnly, Category = Grid)
		TSubclassOf<ARTSStructure> StructureClass;

	private:
		bool bIsBuildGridVisible = false;
};
