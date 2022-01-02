// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExpProgressBar.h"
#include "GameFramework/PlayerState.h"

void UFPSExpProgressBar::NativeConstruct()
{
	Super::NativeConstruct();
	PercentDelegate.BindUFunction(this, "UpdatePercentExp");
}

float UFPSExpProgressBar::UpdatePercentExp()
{
	float retval = 0.0f;
	IExpAccumulatorInterface * expuser = GetExpUser();

	if (expuser != nullptr)
	{
		const float currentexp = (float)expuser->GetCurrentExp();
		const float maxexpforlevel = (float)expuser->GetMaxExpForCurrentLevel();
		if (maxexpforlevel != 0.0f)
		{
			retval = currentexp / maxexpforlevel;
		}

		if (LevelTextBlock != nullptr)
		{
			const FString levelstring = FString::FromInt(expuser->GetCurrentLevel());
			LevelTextBlock->SetText(FText::FromString(levelstring));
		}
	}



	return retval;
}

IExpAccumulatorInterface * UFPSExpProgressBar::GetExpUser() const
{
	APlayerController * pc = GetOwningPlayer();
	checkf(pc, TEXT("UFPSExpProgressBar::GetExpUser : Invalid PlayerController"));
	IExpAccumulatorInterface * retval = pc->GetPlayerState<IExpAccumulatorInterface>();

	return retval;
}
