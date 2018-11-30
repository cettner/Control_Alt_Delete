// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSCamera.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"

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

	//Give control to player 0
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ARTSCamera::BeginPlay()
{
	Super::BeginPlay();
	
	PC = Cast<APlayerController>(GetController());
	PC->GetViewportSize(ScreenSizeX, ScreenSizeY);
}

// Called every frame
void ARTSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AddActorWorldOffset(GetCameraPanDirection() * CamSpeed);

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
	if (NewLocal.Z >= 1000) NewLocal += GetActorForwardVector() * (CameraMove.Z + 50);
	SetActorLocation(NewLocal);
}

void ARTSCamera::Zoom_Out()
{
	CameraMove = CameraMove.GetSafeNormal() * Camera_Speed;
	FVector NewLocal = GetActorLocation();
	if (NewLocal.Z <= 3000) NewLocal += GetActorForwardVector() * (CameraMove.Z - 50);
	SetActorLocation(NewLocal);
}

// Called to bind functionality to input
void ARTSCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARTSCamera::Translate_Cam_Up_Down);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARTSCamera::Translate_Cam_LeftRight);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTSCamera::Zoom_In);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTSCamera::Zoom_Out);

}

FVector ARTSCamera::GetCameraPanDirection()
{
	float MousePosX, MousePosY;
	float CamDirectionX = 0;
	float CamDirectionY = 0;

	PC->GetMousePosition(MousePosX, MousePosY);

	//edit scroll speed
	if (MousePosX <= Margin && MousePosX >= 0)
	{
		CamDirectionY = -1;
	}

	if (MousePosY <= Margin && MousePosY >= 0)
	{
		CamDirectionX = 1;
	}

	if (MousePosX >= ScreenSizeX - Margin)
	{
		CamDirectionY = 1;
	}

	if (MousePosY >= ScreenSizeY - Margin)
	{
		CamDirectionX = -1;
	}

	return(FVector(CamDirectionX, CamDirectionY, 0));
}
