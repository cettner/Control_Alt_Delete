// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "RTSOrder.generated.h"



UCLASS(Blueprintable)
class RTS_PROJECT_API URTSOrder : public UObject
{
	GENERATED_BODY()

	public:
		inline bool RequiresTarget() const;
		inline UTexture* GetThumbnail() const;

	protected:
		UPROPERTY(EditDefaultsOnly)
		UTexture* OrderThumbnail = nullptr;

		UPROPERTY(EditDefaultsOnly)
		bool bRequiresTarget = false;

};
