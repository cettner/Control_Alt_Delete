// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"
#include "Camera/CameraComponent.h"
#include "Commander.generated.h"


UCLASS()
class RTS_PROJECT_API ACommander : public ARTSMinion
{
	GENERATED_BODY()

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* ActorInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

public:
	/*Override to allow selection of which mesh should play the animation*/
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	/*Stop playing montage*/
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

public:
	ACommander();

	//handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Val);

	//handles strafing
	UFUNCTION()
	void MoveRight(float Val);

	bool GetMarchingOrder(ARTSMinion * needs_orders, FVector &Outvector);

	bool AddtoSquad(ARTSMinion * squadmate);

	bool LeaveSquad(ARTSMinion * leaver);

	bool IsFirstPerson() const;

	/**************RTSMinion Overrides************/
	virtual ACommander * GetCommander() override;

	virtual void ClearCommander() override;

	virtual void SetCommander(ACommander * Commander) override;
    
    virtual void SetTarget(AActor * newtarget) override;

	virtual bool CanInteract(AActor * Interactable) override;

	virtual int GetTeam() const override;

	virtual void OnDeath() override;
	/********************************************/


	virtual bool IsServerPawn() const;

	void SetIsServerPawn(bool IsServer);

	UFUNCTION(Server, reliable, WithValidation)
	void MinionInteractionHandler(ARTSMinion * Interacted);

	UFUNCTION(Server, reliable, WithValidation)
	void SelectableInterationHandler(AActor * Interacted);

	USkeletalMeshComponent * GetSpecifcPawnMesh(bool WantFirstPerson) const;

	USkeletalMeshComponent* GetPawnMesh();

	UFUNCTION()
	virtual void Interact();

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> Squad;


protected:
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* FPS_Camera;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* FPS_Mesh;


private:
	bool bIsServerPawn = false;

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
