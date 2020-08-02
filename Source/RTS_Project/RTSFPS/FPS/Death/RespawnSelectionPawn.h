// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RespawnSelectionPawn.generated.h"

UCLASS()
class RTS_PROJECT_API ARespawnSelectionPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARespawnSelectionPawn();

	UFUNCTION(BlueprintCallable)
	virtual void SetRevolveActor(AActor* FollowMe);

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void CyclePrev();
	virtual void CycleNext();
	virtual void RotateCamera(float AxisVal);
	UFUNCTION()
	virtual void SelectSpawnActor();



protected:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* MainCamera;

	FRotator CameraRotation;

	AActor* FollowActor;
};