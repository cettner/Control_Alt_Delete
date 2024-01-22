// Fill out your copyright notice in the Description page of Project Settings.

#include "RTS_Project.h"
#include "GameplayDebugger.h"
#include "RTSFPS/RTS/Minions/AI/Navigation/Debug/GameplayDebuggerCategory_Boid.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FRTSModule, RTS_Project, "RTS_Project" );


void FRTSModule::StartupModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("Boid", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_Boid::MakeInstance));
#endif
}

void FRTSModule::ShutdownModule()
{
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.UnregisterCategory("Boid");
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}
