// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSCamera.h"
#include "Components/InputComponent.h"
// Sets default values
ARTSCamera::ARTSCamera()
{
	bReplicates = false;
	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = !(GetNetMode() == NM_DedicatedServer && GetLocalRole() == ROLE_Authority) ;

	MainCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	MainCameraSpringArm->TargetArmLength = 350.0;
	MainCameraSpringArm->SetWorldRotation(FRotator(0.0, 0.0, 0.0));
	MainCameraSpringArm->bDoCollisionTest = false;

	MainCamera->SetupAttachment(MainCameraSpringArm, USpringArmComponent::SocketName);
	MainCamera->SetRelativeRotation(FRotator(-45.0, 0.0, 0.0));

	// Controls Camera Movement Speed
	CameraSpeed = 800.0f;
}

void ARTSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CameraMove.IsZero())
	{
		CameraMove = CameraMove.GetSafeNormal() * CameraSpeed;
		FVector NewLocal = GetActorLocation();
		FVector XYPlane = FVector::CrossProduct(GetActorForwardVector(), FVector(0.0,0.0,-1.0));
		NewLocal += GetActorForwardVector() * CameraMove.X * DeltaTime;
		NewLocal += XYPlane * CameraMove.Y * DeltaTime;
		SetActorLocation(NewLocal);
	}

}

void ARTSCamera::TranslateCamLeftRight(float AxisVal)
{
	CameraMove.Y = FMath::Clamp<float>(AxisVal, -1.0, 1.0);
}

void ARTSCamera::TranslateCamUpDown(float AxisVal)
{
	CameraMove.X = FMath::Clamp<float>(AxisVal, -1.0, 1.0);
}

void ARTSCamera::ZoomIn()
{
	CameraMove = CameraMove.GetSafeNormal() * CameraSpeed;
	FVector NewLocal = GetActorLocation();
	NewLocal += MainCamera->GetForwardVector() * (CameraMove.Z + 50);
	SetActorLocation(NewLocal);
}

void ARTSCamera::ZoomOut()
{
	CameraMove = CameraMove.GetSafeNormal() * CameraSpeed;
	FVector NewLocal = GetActorLocation();
	NewLocal += MainCamera->GetForwardVector() * (CameraMove.Z - 50);
	SetActorLocation(NewLocal);
}

void ARTSCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARTSCamera::TranslateCamUpDown);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARTSCamera::TranslateCamLeftRight);
	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &ARTSCamera::ZoomIn);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &ARTSCamera::ZoomOut);
}

