// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrder.h"

bool URTSOrder::RequiresTarget() const
{
	return bRequiresTarget;
}

int URTSOrder::GetApplicationCount() const
{
	return PropertyApplicationCount;
}
