// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"


#include "LoadMenuLevel.generated.h"

UCLASS()
class RTS_PROJECT_API ALoadMenuLevel : public ALevelScriptActor
{
	GENERATED_BODY()

		virtual void PreInitializeComponents() override;


#ifdef WITH_EDITOR
	virtual void PostInitializeComponents() override;
#endif // WITH_EDITOR

};
