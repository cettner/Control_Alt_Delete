// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "RTSProperty.generated.h"



UCLASS(Blueprintable)
class RTS_PROJECT_API URTSProperty : public UObject
{
	GENERATED_BODY()

	public:
		inline UTexture* GetThumbnail() const;

		virtual FText GetPropertyDescription() const;

	protected:
		UPROPERTY(EditDefaultsOnly)
		UTexture* PropertyThumbnail = nullptr;

		UPROPERTY(EditDefaultsOnly)
		FText PropertyDescription = FText::FromString(FString("DefaultProperty"));
};
