// Fill out your copyright notice in the Description page of Project Settings.


#include "MageHands.h"
#include "../../CombatCommander.h"

USkeletalMeshComponent* AMageHands::GetWeaponMesh() const
{
	return(MyPawn->GetPawnMesh());
}