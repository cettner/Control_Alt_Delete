// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseClasses/RTSMinion.h"
#include "Camera/CameraComponent.h"
#include "../RTS/Camera/RTSSelectable.h"
#include "Commander.generated.h"

#define SELECTION_CHANNEL  ECC_GameTraceChannel1


UCLASS()
class RTS_PROJECT_API ACommander : public ARTSMinion
{
	GENERATED_BODY()

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* ActorInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

public:
	/*Override to allow selection of which mesh should play the animation*/
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	/*Stop playing montage*/
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

public:
	ACommander();

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* FPS_Camera;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* FPS_Mesh;

	//handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Val);

	//handles strafing
	UFUNCTION()
	void MoveRight(float Val);

	bool GetMarchingOrder(ARTSMinion * needs_orders, FVector &Outvector);

	bool AddtoSquad(ARTSMinion * squadmate);

	bool LeaveSquad(ARTSMinion * leaver);

	bool IsFirstPerson();

	virtual ACommander * GetCommander() override;

	virtual void ClearCommander() override;

	virtual void SetCommander(ACommander * Commander) override;
    
    virtual void SetTarget(AActor * newtarget) override;

	virtual bool CanInteract(AActor * Interactable) override;

	virtual int GetTeam() override;

	UFUNCTION(Server, reliable, WithValidation)
	void MinionInteractionHandler(ARTSMinion * Interacted);

	UFUNCTION(Server, reliable, WithValidation)
	void SelectableInterationHandler(ARTSSelectable * Interacted);

	USkeletalMeshComponent * GetSpecifcPawnMesh(bool WantFirstPerson) const;

	USkeletalMeshComponent* GetPawnMesh();

	UFUNCTION()
	virtual void Interact();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> Squad;

private:
	float marchwidth = 150.0f;

	FCollisionQueryParams trace;

	AActor * GetSelectableActor();

	FVector GetSquareFormation(int index, float width);

	enum FORMATION
	{
		SQUARE
	};

	FORMATION form = SQUARE;
};
