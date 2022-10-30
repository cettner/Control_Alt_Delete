// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSProperty.h"

UTexture2D* URTSProperty::GetThumbnail() const
{
	return PropertyThumbnail;
}

FText URTSProperty::GetPropertyDescription() const
{
	return PropertyDescription;
}

void URTSProperty::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

bool URTSProperty::IsSupportedForNetworking() const
{
	return true;
}

UWorld* URTSProperty::GetWorld() const
{
	if (const UObject* MyOuter = GetOuter())
	{
		return MyOuter->GetWorld();
	}
	return nullptr;
}
