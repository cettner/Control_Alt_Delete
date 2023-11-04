// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "Camera/CameraComponent.h"
#include "Commander.generated.h"


UCLASS()
class RTS_PROJECT_API ACommander : public ARTSMinion
{
	GENERATED_BODY()

public:
	ACommander();

	/*Override to allow selection of which mesh should play the animation*/
	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	/*Stop playing montage*/
	virtual void StopAnimMontage(class UAnimMontage* AnimMontage) override;

	bool AddtoSquad(ARTSMinion * squadmate);

	bool LeaveSquad(ARTSMinion * leaver);

	bool IsFirstPerson() const;

	virtual bool IsServerPawn() const;

	void SetIsServerPawn(bool IsServer);

	UFUNCTION(Server, reliable, WithValidation)
	void MinionInteractionHandler(ARTSMinion* Interacted);

	UFUNCTION(Server, reliable, WithValidation)
	void SelectableInterationHandler(AActor* Interacted);

	USkeletalMeshComponent* GetPawnMesh();

	USkeletalMeshComponent* GetSpecifcPawnMesh(bool WantFirstPerson) const;

protected:

	//handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Val);

	//handles strafing
	UFUNCTION()
	void MoveRight(float Val);

	UFUNCTION()
	virtual void Interact();

	/**************RTSMinion Overrides************/
public:
	virtual IRTSObjectInterface * GetLeadRTSObject() override;

	virtual void ClearCommander() override;

	virtual void SetCommander(ACommander * Commander) override;

	virtual int GetTeam() const override;
	virtual void SetTeam(int InTeamID) override;

	virtual void IssueOrder(AController* Issuer, const FHitResult& InHitContext, URTSOrder* InOrder, const bool InbIsQueuedOrder) override;

protected:
	virtual void OnDeath() override;
	/********************************************/


	/**************RTSMinion Overrides************/
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* ActorInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	/********************************************/

public:
	UPROPERTY(Replicated, EditAnywhere, Category = Gameplay)
	TArray <ARTSMinion*> Squad;

protected:
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* FPS_Camera;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* FPS_Mesh;

private:
	bool bIsServerPawn = false;

	FCollisionQueryParams trace;

	AActor * GetSelectableActor();
};
