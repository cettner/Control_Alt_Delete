// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSSelectionCamera.h"
#include "../Orders/RTSTargetedOrder.h"

#include "RTSOrderCamera.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSOrderCamera : public ARTSSelectionCamera
{
	GENERATED_BODY()
	
	protected:
		virtual void OrderSelected();

		/*Client side Function that is evaluated again in the player controller server side, removes unordable units 
		and prevents a server call if no units are orderable from the selection*/
		virtual TArray<TScriptInterface<IRTSObjectInterface>> GetOrderableUnitsFromSelection() const;

	protected:
		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
