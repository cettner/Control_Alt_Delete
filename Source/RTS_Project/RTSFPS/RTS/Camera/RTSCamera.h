// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSCamera.generated.h"

UCLASS()
class RTS_PROJECT_API ARTSCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSCamera();

protected:

	FVector CameraMove;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Camera Movement Functions
	void TranslateCamLeftRight(float AxisVal);
	void TranslateCamUpDown(float AxisVal);
	void ZoomIn();
	void ZoomOut();



	UCameraComponent* MainCamera;

	float CameraSpeed;
};
