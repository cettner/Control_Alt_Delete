// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialProgressBar.h"

void UMaterialProgressBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	

	if (PercentDelegate.IsBound())
	{
		const float outfloat = PercentDelegate.Execute();
		if (outfloat != Percent)
		{
			SetPercent(outfloat);
		}
	}
}

void UMaterialProgressBar::NativeConstruct()
{
	Super::NativeConstruct();
	LastStablePercent = Percent;
	SetPercent(Percent);
}

void UMaterialProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	static float elapsedtime = 0.0f;

	/*Check if we can get data*/
	if (PercentDelegate.IsBound())
	{
		Percent = PercentDelegate.Execute();
		/*The True Progressbar state has changed so reset the lerp time and save the value so we know when to reset again*/
		if (Percent != LastStablePercent)
		{
			LastStablePercent = Percent;
			elapsedtime = 0.0f;
		}

		/*If the lerp hasnt caught up with the true value*/
		if (LerpPercent != Percent)
		{
			/*Check how long we've interpolated for*/
			if (elapsedtime < LerpDuration)
			{
				/*Lerp Up or Down*/
				if (Percent > LerpPercent)
				{
					LerpPercent = FMath::Lerp(LerpPercent, Percent, elapsedtime / LerpDuration);
					elapsedtime += InDeltaTime;
				}
				else if (Percent < LerpPercent)
				{
					/*Maximum Value Cannot be 0, So increment everything by one*/
					const float decrement =  FMath::Lerp(Percent + 1.0f, LerpPercent + 1.0f, elapsedtime / LerpDuration);
					LerpPercent -= (decrement -1.0f);
					
					elapsedtime += InDeltaTime;
				}
			}
			else
			{
				/*We've lagged as long as we wanted to so set them equal*/
				LerpPercent = Percent;
			}

			SetPercent(LerpPercent);
		}
	}
}

void UMaterialProgressBar::SetPercent(float InPercent)
{
	UMaterialInstanceDynamic * dynamicmat = GetMaterialInterface();

	if (dynamicmat != nullptr)
	{
		dynamicmat->SetScalarParameterValue(FName("Progress"), InPercent);
	}
}

UMaterialInstanceDynamic * UMaterialProgressBar::GetMaterialInterface() const
{
	UMaterialInstanceDynamic * retval = nullptr;

	if (ProgressBarImage != nullptr)
	{
		retval = ProgressBarImage->GetDynamicMaterial();
	}

	return retval;
}
