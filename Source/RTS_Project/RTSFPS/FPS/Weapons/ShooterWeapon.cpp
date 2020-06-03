// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterWeapon.h"
#include "RTSFPS/FPS/CombatCommander.h"
#include "AIController.h"
#define DEBUG_WEAPON

AShooterWeapon::AShooterWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentAmmo = WeaponConfig.InitialClips * WeaponConfig.AmmoPerClip;
}

void AShooterWeapon::StartFire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerStartFire();
	}

	if (!bWantsToFire)
	{
		bWantsToFire = true;
		DetermineWeaponState();
	}
}

void AShooterWeapon::StopFire()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerStopFire();
	}

	if (bWantsToFire)
	{
		bWantsToFire = false;
		DetermineWeaponState();
	}
}

void AShooterWeapon::StartReload(bool bFromReplication)
{
	if (!bFromReplication && GetLocalRole() < ROLE_Authority)
	{
		ServerStartReload();
	}

	if (bFromReplication || CanReload())
	{
		bPendingReload = true;
		DetermineWeaponState();

		float AnimDuration = PlayWeaponAnimation(ReloadAnim);
		if (AnimDuration <= 0.0f)
		{
			AnimDuration = WeaponConfig.NoAnimReloadDuration;
		}

		GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &AShooterWeapon::StopReload, AnimDuration, false);
		if (GetLocalRole() == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_ReloadWeapon, this, &AShooterWeapon::ReloadWeapon, FMath::Max(0.1f, AnimDuration - 0.1f), false);
		}

	//	if (MyPawn && MyPawn->IsLocallyControlled())
	//	{
	//		PlayWeaponSound(ReloadSound);
	//	}
	}
}

void AShooterWeapon::StopReload()
{
	if (CurrentState == EWeaponState::Reloading)
	{
		bPendingReload = false;
		DetermineWeaponState();
		StopWeaponAnimation(ReloadAnim);
	}
}

void AShooterWeapon::ReloadWeapon()
{
	int32 ClipDelta = FMath::Min(WeaponConfig.AmmoPerClip - CurrentAmmoInClip, CurrentAmmo - CurrentAmmoInClip);

	if (HasInfiniteClip())
	{
		ClipDelta = WeaponConfig.AmmoPerClip - CurrentAmmoInClip;
	}

	if (ClipDelta > 0)
	{
		CurrentAmmoInClip += ClipDelta;
	}

	if (HasInfiniteClip())
	{
		CurrentAmmo = FMath::Max(CurrentAmmoInClip, CurrentAmmo);
	}
}

AShooterWeapon::EAmmoType AShooterWeapon::GetAmmoType() const
{
	return(EAmmoType::EBullet);
}

void AShooterWeapon::UseAmmo()
{
	if (!HasInfiniteClip())
	{
		CurrentAmmoInClip--;
	}

	if (!HasInfiniteAmmo() && !HasInfiniteClip())
	{
		CurrentAmmo--;
	}

	/*
	AShooterAIController* BotAI = MyPawn ? Cast<AShooterAIController>(MyPawn->GetController()) : NULL;
	AShooterPlayerController* PlayerController = MyPawn ? Cast<AShooterPlayerController>(MyPawn->GetController()) : NULL;
	if (BotAI)
	{
		BotAI->CheckAmmo(this);
	}
	else if (PlayerController)
	{
		AShooterPlayerState* PlayerState = Cast<AShooterPlayerState>(PlayerController->PlayerState);
		switch (GetAmmoType())
		{
		case EAmmoType::ERocket:
			PlayerState->AddRocketsFired(1);
			break;
		case EAmmoType::EBullet:
		default:
			PlayerState->AddBulletsFired(1);
			break;
		}
	}
	*/
}

void AShooterWeapon::HandleFiring()
{
	if ((CurrentAmmoInClip > 0 || HasInfiniteClip() || HasInfiniteAmmo()) && CanFire())
	{
		if (GetNetMode() != NM_DedicatedServer)
		{
			SimulateWeaponFire();
		}

		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			FireWeapon();

			UseAmmo();

			// update firing FX on remote clients if function was called on server
			BurstCounter++;
		}
	}
	else if (CanReload())
	{
		StartReload();
	}
	else if (MyPawn && MyPawn->IsLocallyControlled())
	{
		if (GetCurrentAmmo() == 0 && !bRefiring)
		{
			/*
			PlayWeaponSound(OutOfAmmoSound);
			AShooterPlayerController* MyPC = Cast<AShooterPlayerController>(MyPawn->Controller);
			if (MyPC)
			{
				MyPC->NotifyOutOfAmmo();
			}
			*/
		}

		// stop weapon fire FX, but stay in Firing state
		if (BurstCounter > 0)
		{
			OnBurstFinished();
		}
	}

	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		// local client will notify server
		if (GetLocalRole() < ROLE_Authority)
		{
			ServerHandleFiring();
		}

		// reload after firing last round
		if (CurrentAmmoInClip <= 0 && CanReload())
		{
			StartReload();
		}

		// setup refire timer
		bRefiring = (CurrentState == EWeaponState::Firing && WeaponConfig.TimeBetweenShots > 0.0f);
		if (bRefiring)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AShooterWeapon::HandleFiring, WeaponConfig.TimeBetweenShots, false);
		}
	}

	LastFireTime = GetWorld()->GetTimeSeconds();
}

void AShooterWeapon::OnBurstStarted()
{
	// start firing, can be delayed to satisfy TimeBetweenShots
	const float GameTime = GetWorld()->GetTimeSeconds();
	if (LastFireTime > 0 && WeaponConfig.TimeBetweenShots > 0.0f &&
		LastFireTime + WeaponConfig.TimeBetweenShots > GameTime)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AShooterWeapon::HandleFiring, LastFireTime + WeaponConfig.TimeBetweenShots - GameTime, false);
	}
	else
	{
		HandleFiring();
	}
}

void AShooterWeapon::OnBurstFinished()
{
	// stop firing FX on remote clients
	BurstCounter = 0;

	// stop firing FX locally, unless it's a dedicated server
	if (GetNetMode() != NM_DedicatedServer)
	{
		StopSimulatingWeaponFire();
	}

	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
	bRefiring = false;
}

void AShooterWeapon::SimulateWeaponFire()
{
	if (GetLocalRole() == ROLE_Authority && CurrentState != EWeaponState::Firing)
	{
		return;
	}

	if (!bLoopedFireAnim || !bPlayingFireAnim)
	{
		PlayWeaponAnimation(FireAnim);
		bPlayingFireAnim = true;
	}

	/*
	if (MuzzleFX)
	{
		USkeletalMeshComponent* UseWeaponMesh = GetWeaponMesh();
		MuzzlePSC = UGameplayStatics::SpawnEmitterAttached(MuzzleFX, UseWeaponMesh, MuzzleAttachPoint);
	}

	if (bLoopedFireSound)
	{
		if (FireAC == NULL)
		{
			FireAC = PlayWeaponSound(FireLoopSound);
		}
	}
	else
	{
		PlayWeaponSound(FireSound);
	}

	AShooterPlayerController* PC = (MyPawn != NULL) ? Cast<AShooterPlayerController>(MyPawn->Controller) : NULL;
	if (PC != NULL && PC->IsLocalController())
	{
		if (FireCameraShake != NULL)
		{
			PC->ClientPlayCameraShake(FireCameraShake, 1);
		}
		if (FireForceFeedback != NULL && PC->IsVibrationEnabled())
		{
			FForceFeedbackParameters FFParams;
			FFParams.bLooping = false;
			FFParams.bPlayWhilePaused = false;
			FFParams.Tag = "Weapon";
			PC->ClientPlayForceFeedback(FireForceFeedback, FFParams);
		}
	}
	*/
}

void AShooterWeapon::StopSimulatingWeaponFire()
{

	if (bLoopedFireAnim && bPlayingFireAnim)
	{
		StopWeaponAnimation(FireAnim);
		bPlayingFireAnim = false;
	}
	/*
	if (bLoopedMuzzleFX)
	{
		if (MuzzlePSC != NULL)
		{
			MuzzlePSC->DeactivateSystem();
			MuzzlePSC = NULL;
		}
		if (MuzzlePSCSecondary != NULL)
		{
			MuzzlePSCSecondary->DeactivateSystem();
			MuzzlePSCSecondary = NULL;
		}
	}

	if (FireAC)
	{
		FireAC->FadeOut(0.1f, 0.0f);
		FireAC = NULL;

		PlayWeaponSound(FireFinishSound);
	}
	*/
}

void AShooterWeapon::DetermineWeaponState()
{
	EWeaponState::Type NewState = EWeaponState::Idle;

	if (bPendingUnEquip)
	{
		NewState = EWeaponState::Unequipping;
	}
	else if (bPendingEquip)
	{
		NewState = EWeaponState::Equipping;
	}
	else if (!bIsEquipped)
	{
		NewState = EWeaponState::Unequipped;
	}
	else if (bIsEquipped)
	{
		if (bPendingReload)
		{
			if (CanReload() == false)
			{
				NewState = CurrentState;
			}
			else
			{
				NewState = EWeaponState::Reloading;
			}
		}
		else if ((bPendingReload == false) && (bWantsToFire == true) && (CanFire() == true))
		{
			NewState = EWeaponState::Firing;
		}
	}

	SetWeaponState(NewState);
}

void AShooterWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	const EWeaponState::Type PrevState = CurrentState;

	if (PrevState == EWeaponState::Firing && NewState != EWeaponState::Firing)
	{
		OnBurstFinished();
	}

	CurrentState = NewState;

	if (PrevState != EWeaponState::Firing && NewState == EWeaponState::Firing)
	{
		OnBurstStarted();
	}
}

FVector AShooterWeapon::GetCameraDamageStartLocation(const FVector& AimDir) const
{
	APlayerController * PC = MyPawn ? Cast<APlayerController>(MyPawn->Controller) : NULL;
	AAIController* AIPC = MyPawn ? Cast<AAIController>(MyPawn->Controller) : NULL;
	FVector OutStartTrace = this->GetActorLocation();

	if (PC)
	{
		// use player's camera
		FRotator UnusedRot;
		PC->GetPlayerViewPoint(OutStartTrace, UnusedRot);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		OutStartTrace = OutStartTrace + AimDir * ((GetInstigator()->GetActorLocation() - OutStartTrace) | AimDir);
	}
	else if (AIPC)
	{
		OutStartTrace = GetMuzzleLocation();
	}
	
	return(OutStartTrace);
}

FVector AShooterWeapon::GetMuzzleLocation() const
{
	USkeletalMeshComponent* UseMesh = GetWeaponMesh();
	return(UseMesh->GetSocketLocation(MuzzleAttachPoint));
}

FVector AShooterWeapon::GetMuzzleDirection() const
{
	USkeletalMeshComponent* UseMesh = GetWeaponMesh();
	return(UseMesh->GetSocketRotation(MuzzleAttachPoint).Vector());
}

FHitResult AShooterWeapon::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetInstigator());
	TraceParams.bReturnPhysicalMaterial = true;

#ifdef DEBUG_WEAPON
	const FName TraceTag("DebugShooterTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	TraceParams.TraceTag = TraceTag;
#endif // Define to view weapon trace
	
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON, TraceParams);

	return(Hit);
}

int32 AShooterWeapon::GetCurrentAmmo() const
{
	return(CurrentAmmo);
}

int32 AShooterWeapon::GetCurrentAmmoInClip() const
{
	return(CurrentAmmoInClip);
}

int32 AShooterWeapon::GetAmmoPerClip() const
{
	return(WeaponConfig.AmmoPerClip);
}

int32 AShooterWeapon::GetMaxAmmo() const
{
	return(WeaponConfig.MaxAmmo);
}

bool AShooterWeapon::CanFire() const
{
	//bool bCanFire = MyPawn && MyPawn->CanFire();
	bool bStateOKToFire = ((CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing));
	return ((bStateOKToFire == true) && (bPendingReload == false));
}

bool AShooterWeapon::CanReload() const
{
	//bool bCanReload = (!MyPawn || MyPawn->CanReload());
	bool bGotAmmo = ((CurrentAmmoInClip < WeaponConfig.AmmoPerClip) && (CurrentAmmo - CurrentAmmoInClip > 0) && !HasInfiniteClip());
	bool bStateOKToReload = ((CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing));
	return(bGotAmmo && bStateOKToReload);
}

bool AShooterWeapon::HasInfiniteAmmo() const
{
	return(WeaponConfig.bInfiniteAmmo);
}

bool AShooterWeapon::HasInfiniteClip() const
{
	return(WeaponConfig.bInfiniteClip);
}

void AShooterWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AShooterWeapon, CurrentAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AShooterWeapon, CurrentAmmoInClip, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AShooterWeapon, BurstCounter, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AShooterWeapon, bPendingReload, COND_SkipOwner);
}

void AShooterWeapon::OnRep_Reload()
{
	if (bPendingReload)
	{
		StartReload(true);
	}
	else
	{
		StopReload();
	}
}

void AShooterWeapon::OnRep_BurstCounter()
{
	if (BurstCounter > 0)
	{
		SimulateWeaponFire();
	}
	else
	{
		StopSimulatingWeaponFire();
	}
}

bool AShooterWeapon::ServerHandleFiring_Validate()
{
	return true;
}

void AShooterWeapon::ServerHandleFiring_Implementation()
{
	const bool bShouldUpdateAmmo = (CurrentAmmoInClip > 0 && CanFire());

	HandleFiring();

	if (bShouldUpdateAmmo)
	{
		// update ammo
		UseAmmo();

		// update firing FX on remote clients
		BurstCounter++;
	}
}

bool AShooterWeapon::ServerStartFire_Validate()
{
	return(true);
}

void AShooterWeapon::ServerStartFire_Implementation()
{
	StartFire();
}

bool AShooterWeapon::ServerStopFire_Validate()
{
	return true;
}

void AShooterWeapon::ServerStopFire_Implementation()
{
	StopFire();
}

bool AShooterWeapon::ServerStartReload_Validate()
{
	return true;
}

void AShooterWeapon::ServerStartReload_Implementation()
{
	StartReload();
}

bool AShooterWeapon::ServerStopReload_Validate()
{
	return true;
}

void AShooterWeapon::ServerStopReload_Implementation()
{
	StopReload();
}

void AShooterWeapon::ClientStartReload_Implementation()
{
	StartReload();
}
