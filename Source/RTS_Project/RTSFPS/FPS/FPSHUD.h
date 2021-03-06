// Fill out your copyright notice in the Description page of Project Settings.



#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSHUD.h"
#include "UI/FPSUI.h"
#include "FPSHUD.generated.h"

 

UCLASS()
class RTS_PROJECT_API AFPSHUD : public ARTSHUD
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UFPSUI> FPSUIClass;
	
};



