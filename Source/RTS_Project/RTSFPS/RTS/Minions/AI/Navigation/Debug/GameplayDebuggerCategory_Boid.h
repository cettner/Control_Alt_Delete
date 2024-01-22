#pragma once

#include "CoreMinimal.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerCategory.h"

class AActor;
class APlayerController;

class FGameplayDebuggerCategory_Boid : public FGameplayDebuggerCategory
{

public:
	FGameplayDebuggerCategory_Boid();
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;
	RTS_PROJECT_API static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	TMap<FString, FVector> ForceMap = TMap<FString, FVector>();
};

#endif // WITH_GAMEPLAY_DEBUGGER_MENU
