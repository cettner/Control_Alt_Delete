// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSMinion.h"
#include "Camera/CameraComponent.h"
#include "Commander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ACommander : public ARTSMinion
{
	GENERATED_BODY()

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void Tick(float DeltaTime) override;

public:
	ACommander();
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* FPS_Camera;

	//handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Val);
	//handles strafing
	UFUNCTION()
	void MoveRight(float Val);

	int GetMarchingOrder(ARTSMinion * needs_orders);

	bool AddtoSquad(ARTSMinion * squadmate);
	bool LeaveSquad(ARTSMinion * leaver);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> Squad;

private:
	int marching_order = 0;

};
