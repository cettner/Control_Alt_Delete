// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSMinion.h"
#include "Camera/CameraComponent.h"
#include "RTSSelectable.h"
#include "Commander.generated.h"

#define SELECTION_CHANNEL  ECC_GameTraceChannel1


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

	bool GetMarchingOrder(ARTSMinion * needs_orders, FVector &Outvector);

	bool AddtoSquad(ARTSMinion * squadmate);

	bool LeaveSquad(ARTSMinion * leaver);

	virtual ACommander * GetCommander() override;

	virtual void ClearCommander() override;

	virtual void SetCommander(ACommander * Commander) override;
    
    virtual void SetTarget(AActor * newtarget) override;

	UFUNCTION()
	virtual void Interact();

	UFUNCTION()
	void PrimaryPressed();

	UFUNCTION()
	void PrimaryReleased();

	UFUNCTION()
	void SecondaryPressed();

	UFUNCTION() 
	void SecondaryReleased();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> Squad;

	UPROPERTY(BlueprintReadOnly)
	bool Swing_Weapon = false;

	UPROPERTY(BlueprintReadOnly)
	bool blocking = false;

private:
	float marchwidth = 150.0f;

	FCollisionQueryParams trace;

	AActor * GetSelectableActor();

	FVector GetSquareFormation(int index, float width);

	void MinionInteractionHandler(ARTSMinion * Interacted);
	
	void SelectableInterationHandler(ARTSSelectable * Interacted);

	enum FORMATION
	{
		SQUARE
	};

	FORMATION form = SQUARE;
};
