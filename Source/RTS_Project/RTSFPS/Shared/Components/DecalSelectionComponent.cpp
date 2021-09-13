// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalSelectionComponent.h"

UDecalSelectionComponent::UDecalSelectionComponent() : Super()
{
	ColorParamName = "Color";
	SelectionChannel = SELECTION_CHANNEL;
	DecalSize = FVector(300.0f, 300.0f, 300.0f);
	SetRelativeScale3D(FVector(1.0f, .25f, .25f));
	SetRelativeLocation(FVector(0, 0, -100.0f));
	SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
}

void UDecalSelectionComponent::SetSelectionColor(FLinearColor Color)
{
	if (GetDecalMaterial() == nullptr) return;

	if (GetDecalMaterial()->IsA(UMaterialInstanceDynamic::StaticClass()))
	{
		UMaterialInstanceDynamic * DynamicColor = Cast<UMaterialInstanceDynamic>(GetDecalMaterial());
		DynamicColor->SetVectorParameterValue(ColorParamName, Color);
	}
	else
	{
		UMaterialInstanceDynamic * DynamicColor = UMaterialInstanceDynamic::Create(GetDecalMaterial(), this);
		if (DynamicColor)
		{
			DynamicColor->SetVectorParameterValue(ColorParamName, Color);
			SetDecalMaterial(DynamicColor);
		}
	}
}

void UDecalSelectionComponent::SetDetection(UPrimitiveComponent * Collision)
{
	Collision->SetCollisionResponseToChannel(SelectionChannel, ECR_Block);
}