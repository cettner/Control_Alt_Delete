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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector CameraMove;
	FVector2D CameraPos;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	//Camera Movement Functions
	void Translate_Cam_LeftRight(float AxisVal);
	void Translate_Cam_Up_Down(float AxisVal);
	void Zoom_In();
	void Zoom_Out();

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* Main_CameraSpringArm;
	UCameraComponent* Main_Camera;
	//USceneComponent* Selected_Mesh;  //FIGURE THIS OUT
	float Camera_Speed;

	
};
