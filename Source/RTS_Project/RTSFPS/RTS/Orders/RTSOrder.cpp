// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrder.h"

inline bool URTSOrder::RequiresTarget() const
{
	return bRequiresTarget;
}


inline UTexture* URTSOrder::GetThumbnail() const
{
	return OrderThumbnail;
}
