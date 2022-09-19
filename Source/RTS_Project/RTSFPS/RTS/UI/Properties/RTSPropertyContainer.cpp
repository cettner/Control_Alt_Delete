// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPropertyContainer.h"



const TArray<TSubclassOf<URTSProperty>> URTSPropertyContainer::GetNestedProperties() const
{
	return NestedProperties;
}