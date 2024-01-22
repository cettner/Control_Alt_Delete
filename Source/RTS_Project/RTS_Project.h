// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class RTS_PROJECT_API FRTSModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};