// Fill out your copyright notice in the Description page of Project Settings.


#include "ExternalWidget.h"
#include "..\GameArchitecture\DefaultHUD.h"

FStackWidgetInfo UExternalWidget::GetWidgetInputSettings() const
{
	return DefaultInputSettings;
}

FReply UExternalWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    FReply retval = FReply::Unhandled();

    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        retval = FReply::Handled();
        OnEscapeAction();
    }

    return retval;
}

/*
void UExternalWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
    Super::AddToScreen(LocalPlayer, ZOrder);
   
    bIsFocusable = true;
    SetFocus();
}
*/

void UExternalWidget::OnEscapeAction()
{
    ADefaultHUD* hud = GetOwningPlayer()->GetHUD<ADefaultHUD>();
    hud->OnEscapeAction();
}
