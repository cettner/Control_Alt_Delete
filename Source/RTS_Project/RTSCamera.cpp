// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSCamera.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.h"
// Sets default values
ARTSCamera::ARTSCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Main_CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Main_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	Main_CameraSpringArm->TargetArmLength = 350.0;
	Main_CameraSpringArm->SetWorldRotation(FRotator(0.0, 0.0, 0.0));
	Main_CameraSpringArm->bDoCollisionTest = false;

	Main_Camera->SetupAttachment(Main_CameraSpringArm, USpringArmComponent::SocketName);

	// Controls Camera Movement Speed
	Camera_Speed = 800.0f;
}

// Called when the game starts or when spawned
void ARTSCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARTSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CameraMove.IsZero())
	{
		CameraMove = CameraMove.GetSafeNormal() * Camera_Speed;
		FVector NewLocal = GetActorLocation();
		NewLocal += FVector(1.0, 0.0, 0.0) * CameraMove.X * DeltaTime;
		NewLocal += FVector(0.0, 1.0, 0.0) * CameraMove.Y * DeltaTime;
		NewLocal += GetActorForwardVector() * CameraMove.Z * DeltaTime;
		SetActorLocation(NewLocal);
	}

}

void ARTSCamera::Translate_Cam_LeftRight(float AxisVal)
{
	CameraMove.Y = FMath::Clamp<float>(AxisVal, -1.0, 1.0);
}

void ARTSCamera::Translate_Cam_Up_Down(float AxisVal)
{
	CameraMove.X = FMath::Clamp<float>(AxisVal, -1.0, 1.0);
}

void ARTSCamera::Zoom_In()
{
	CameraMove = CameraMove.GetSafeNormal() * Camera_Speed;
	FVector NewLocal = GetActorLocation();
	NewLocal += GetActorForwardVector() * (CameraMove.Z + 50);
	SetActorLocation(NewLocal);
}

void ARTSCamera::Zoom_Out()
{
	CameraMove = CameraMove.GetSafeNormal() * Camera_Speed;
	FVector NewLocal = GetActorLocation();
	NewLocal += GetActorForwardVector() * (CameraMove.Z - 50);
	SetActorLocation(NewLocal);
}

// Called to bind functionality to input
void ARTSCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARTSCamera::Translate_Cam_Up_Down);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARTSCamera::Translate_Cam_LeftRight);
	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &ARTSCamera::Zoom_In);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &ARTSCamera::Zoom_Out);
}

