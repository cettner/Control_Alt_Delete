// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSCamera.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTSSelectionCamera.generated.h"

/**
 * 
 */
#define SELECTION_CHANNEL  ECC_GameTraceChannel1

DECLARE_DELEGATE_OneParam(RTSSelectionUpdateDelegate, const TArray<TScriptInterface<IRTSObjectInterface>>);

enum ESelectionType
{
	NotSelecting,
	SingleSelect,
	BoxSelect,
	DoubleSelect,

};


UCLASS()
class RTS_PROJECT_API ARTSSelectionCamera : public ARTSCamera
{
	GENERATED_BODY()
	
	public:
		//virtual void LoadNewOrder(const URTSOrder * )
		const TArray <TScriptInterface<IRTSObjectInterface>>& GetSelectedUnits() const;


	protected:
	// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	protected:
		virtual void SelectPressed();
		virtual void SelectReleased();
		virtual void SelectDoublePressed();

		void ProcessSelection();

		void AddObjectToSelection(IRTSObjectInterface* InObj);
		void AddObjectToSelection(TArray<IRTSObjectInterface*> InObjs);
		int32 ClearSelection();

	protected:
		virtual void HandleBoxSelect();
		virtual void BoxSelectionStart();
		virtual void BoxSelectionEnd();
		virtual void HandleDoubleSelect();
		virtual void HandleSingleSelect();

	public:
		RTSSelectionUpdateDelegate SelectionUpdateDelegate = RTSSelectionUpdateDelegate();

	/*Config*/
	protected:
		/*Time in Seconds The mouse must be pressed to enable box selection*/
		UPROPERTY(EditDefaultsOnly)
		float BoxSelectDelayBuffer = .2f;

	/*Runtime*/
	protected:
		/*Since Box Selection is Delayed, this is used to capture the inital mouse click Position*/
		FVector2D InitialMouseScreenPosition = FVector2D();
		FTimerHandle SelectionTimerHandle = FTimerHandle();
		ESelectionType CurrentSelectionType = NotSelecting;
		TArray <TScriptInterface<IRTSObjectInterface>> SelectedUnits = TArray< TScriptInterface<IRTSObjectInterface>>();


};
