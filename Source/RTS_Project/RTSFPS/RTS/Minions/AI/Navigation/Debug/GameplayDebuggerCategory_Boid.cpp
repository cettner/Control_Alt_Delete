
#include "GameplayDebuggerCategory_Boid.h"
#include "../BoidPathFollowingComponent.h"

#include "AIController.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
FGameplayDebuggerCategory_Boid::FGameplayDebuggerCategory_Boid()
{
}

void FGameplayDebuggerCategory_Boid::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	if (const APawn* MyPawn = Cast<APawn>(DebugActor))
	{
		if (const AAIController* AIC = MyPawn->GetController<AAIController>())
		{
			if (UBoidPathFollowingComponent* boidcomponent = Cast<UBoidPathFollowingComponent>(AIC->GetPathFollowingComponent()))
			{
				boidcomponent->DescribeSelfToGameplayDebugger(this);
			}
		}
	}
}

void FGameplayDebuggerCategory_Boid::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	for (const TPair<FString, FVector>& keyval : ForceMap)
	{
		CanvasContext.Printf(TEXT("%s: {yellow}%s : {yellow}%f"), *keyval.Key, *keyval.Value.ToString(), keyval.Value.Length());
	}
}

RTS_PROJECT_API TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_Boid::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_Boid());
}
#endif