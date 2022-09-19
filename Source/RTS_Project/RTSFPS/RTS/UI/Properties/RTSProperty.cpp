// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSProperty.h"

inline UTexture* URTSProperty::GetThumbnail() const
{
	return PropertyThumbnail;
}


FText URTSProperty::GetPropertyDescription() const
{
	return PropertyDescription;
}