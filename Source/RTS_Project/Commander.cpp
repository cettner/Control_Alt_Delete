// Fill out your copyright notice in the Description page of Project Settings.
#include "Commander.h"
#include "Components/SkeletalMeshComponent.h"




ACommander::ACommander()
{
	FPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FPS_Camera->SetupAttachment(GetMesh(), TEXT("Head"));
}

