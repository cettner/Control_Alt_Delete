// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RTSUI.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		virtual void Setup();
};
