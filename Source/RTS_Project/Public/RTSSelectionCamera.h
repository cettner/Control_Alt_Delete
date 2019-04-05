// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSCamera.h"
#include "RTSMinion.h"
#include "RTSStructure.h"
#include "RTSSelectable.h"
#include "RTSSelectionCamera.generated.h"

/**
 * 
 */
#define SELECTION_CHANNEL  ECC_GameTraceChannel1

UCLASS()
class RTS_PROJECT_API ARTSSelectionCamera : public ARTSCamera
{
	GENERATED_BODY()
	
	public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> SelectedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSStructure*> SelectedStructures;

	private:
	void SelectPressed();
	void SelectReleased();
	void MoveSelected();

	ARTSSelectable * TempClick = nullptr;

	class ViewActor
	{
	public:
		ViewActor()
		{
			Selectable = nullptr;
			Minion = nullptr;
		}
		void empty() { Selectable = nullptr; Minion = nullptr; }
		void set(ARTSSelectable * inselect) 
		{
			Selectable = inselect;
			Minion = nullptr;
		};
		void set(ARTSMinion * inMinion)
		{
			Minion = inMinion;
			Selectable = nullptr;
		}
		ARTSSelectable * GetSelectable()
		{
			return (Selectable);
		}
		ARTSMinion * GetMinion()
		{
			return(Minion);
		}
	private:
		ARTSSelectable * Selectable;
		ARTSMinion * Minion;

	};
	ViewActor CurrentView = ViewActor();
	
	
};
