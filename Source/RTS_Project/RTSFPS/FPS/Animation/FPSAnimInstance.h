// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../Weapons/Weapon.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPSAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	virtual void UpdateAimOffset(APawn * AnimPawn);

/**************************First Person Mesh***************************/
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsMoving = false;
/***********************************************************************/


/**************************Third Person Mesh***************************/
protected:
	UPROPERTY(BlueprintReadOnly)
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float Direction = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float Pitch = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float Yaw = 0.0f;
/***********************************************************************/


/*******************************Shared**********************************/
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<Combat_Stance> WeaponStance = NO_WEAPON_STANCE;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling = false;
/***********************************************************************/
};

