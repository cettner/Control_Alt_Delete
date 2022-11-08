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

	public:
		virtual void LoadOrder(const URTSTargetedOrder * InOrder);
		virtual const URTSTargetedOrder * GetOrder() const;
		virtual void ClearOrder();
	
	protected:
		virtual void OrderSelected();

	protected:
		virtual void SelectPressed() override;

	protected:
		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	protected:
		const URTSTargetedOrder* EnqueuedOrder = nullptr;
};
