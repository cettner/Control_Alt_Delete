// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExternalWidgetInterface.generated.h"



UENUM()
enum EWidgetStackOperation
{
	/*Widget Never Overrides Input*/
	INACTIVE,
	/*Widget overrides input when It is Pushed On the ViewPort*/
	ACIVEONPUSH,
	/*Widget Overrides Input when it is popped to*/
	ACTIVEONPOPPED,
	/*Widget Overrides Input on Push and Popped to*/
	ALWAYSACTIVE,
};

UENUM()
enum EWisgetStackInputType
{
	GAMEONLY,
	UIONLY,
	GAMEANDUI
};

USTRUCT()
struct FStackWidgetInfo
{
	GENERATED_USTRUCT_BODY()


	public:
		UPROPERTY(EditDefaultsOnly)
		bool bIsCursorVisible = false;

		UPROPERTY(EditDefaultsOnly)
		bool bConsumeCaptureMouseDown = false;

		UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EMouseCursor::Type> CursorType = EMouseCursor::Default;

		UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EWisgetStackInputType> InputType = EWisgetStackInputType::GAMEANDUI;

		UPROPERTY(EditDefaultsOnly)
		TEnumAsByte <EWidgetStackOperation> StackSettings = EWidgetStackOperation::INACTIVE;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UExternalWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IExternalWidgetInterface
{
	GENERATED_BODY()

	public:
		virtual FStackWidgetInfo GetWidgetInputSettings() const;

};
