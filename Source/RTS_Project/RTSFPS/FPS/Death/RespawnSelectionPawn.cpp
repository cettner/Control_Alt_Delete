// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnSelectionPawn.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "../FPSServerController.h"

// Sets default values
ARespawnSelectionPawn::ARespawnSelectionPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = false;
	bNetUseOwnerRelevancy = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Fulcrum"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1500.0f;
	SpringArmComp->SetRelativeRotation(FRotator(-45.0, 0.0, 0.0));
	SpringArmComp->bDoCollisionTest = false;
	
	SpringArmComp->bInheritYaw = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bInheritPitch = false;

	MainCamera->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	FollowActor = nullptr;
}

// Called to bind functionality to input
void ARespawnSelectionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("QKey", IE_Pressed, this, &ARespawnSelectionPawn::CyclePrev);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ARespawnSelectionPawn::CycleNext);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARespawnSelectionPawn::RotateCamera);
	PlayerInputComponent->BindAction("KKey", IE_Pressed, this, &ARespawnSelectionPawn::SelectSpawnActor);
}

void ARespawnSelectionPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		UWorld * world = GetWorld();

		const APlayerController * pc = GetOwner<APlayerController>();
		const ARTFPSPlayerState * ps = Cast<ARTFPSPlayerState>(pc->PlayerState);
		TArray<ARTSStructure *> teamstructures = ps->GetTeamStructures();

		if (teamstructures.Num() > 0)
		{
			SetRevolveActor(teamstructures[0]);
		}
	}
}

void ARespawnSelectionPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CameraRotation.IsZero())
	{
		CameraRotation = CameraRotation;
		SpringArmComp->AddLocalRotation(CameraRotation);
	}

}

void ARespawnSelectionPawn::OnRep_Controller()
{
	UWorld * world = GetWorld();

	const APlayerController * pc = GetController<APlayerController>();

	const ARTFPSPlayerState * ps = Cast<ARTFPSPlayerState>(pc->PlayerState);

	TArray<ARTSStructure *> teamstructures = ps->GetTeamStructures();
	
	if (teamstructures.Num() > 0)
	{
		SetRevolveActor(teamstructures[0]);
	}
}

void ARespawnSelectionPawn::SetRevolveActor(AActor* FollowMe)
{
	if (FollowMe == nullptr) return;
	FollowActor = FollowMe;
	SetActorLocation(FollowMe->GetActorLocation());
}

void ARespawnSelectionPawn::RotateCamera(float AxisVal)
{
	CameraRotation.Yaw = FMath::Clamp<float>(AxisVal, -1.0, 1.0);
	CameraRotation.Roll = FMath::Clamp<float>(AxisVal, -1.0, 1.0);
}

void ARespawnSelectionPawn::SelectSpawnActor()
{
	AFPSServerController * PC = GetController<AFPSServerController>();
	ARTSStructure* structure = Cast<ARTSStructure>(FollowActor);

	if (PC != nullptr  && structure != nullptr)
	{
		PC->SelectRespawnStructure(structure);
	}

}

void ARespawnSelectionPawn::CyclePrev()
{
}

void ARespawnSelectionPawn::CycleNext()
{
}

