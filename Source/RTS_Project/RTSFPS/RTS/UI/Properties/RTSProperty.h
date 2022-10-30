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
		UTexture2D* GetThumbnail() const;

		virtual FText GetPropertyDescription() const;

protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
		virtual bool IsSupportedForNetworking() const override;
		virtual UWorld* GetWorld() const override;

	protected:
		UPROPERTY(EditDefaultsOnly)
		UTexture2D* PropertyThumbnail = nullptr;

		UPROPERTY(EditDefaultsOnly)
		FText PropertyDescription = FText::FromString(FString("DefaultProperty"));
};
