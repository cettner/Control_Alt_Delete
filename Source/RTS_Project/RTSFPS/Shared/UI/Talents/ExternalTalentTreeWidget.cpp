// Fill out your copyright notice in the Description page of Project Settings.


#include "ExternalTalentTreeWidget.h"
#include "RTS_Project/GameArchitecture/Game/DefaultHUD.h"

UExternalTalentTreeWidget::UExternalTalentTreeWidget() : Super()
{
    DefaultInputSettings.bIsCursorVisible = true;
    DefaultInputSettings.CursorType = EMouseCursor::Hand;
    DefaultInputSettings.InputType = EWisgetStackInputType::GAMEANDUI;
    DefaultInputSettings.StackSettings = EWidgetStackOperation::ACIVEONPUSH;

}

FStackWidgetInfo UExternalTalentTreeWidget::GetWidgetInputSettings() const
{
	return DefaultInputSettings;
}

FReply UExternalTalentTreeWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    FReply retval = FReply::Unhandled();

    if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::N)
    {
        retval = FReply::Handled();
        OnEscapeAction();
    }

    return retval;
}

void UExternalTalentTreeWidget::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
    Super::AddToScreen(LocalPlayer, ZOrder);
    RefreshNodes();
    bIsFocusable = true;
    SetFocus();
}

void UExternalTalentTreeWidget::OnEscapeAction()
{
    ADefaultHUD* hud = GetOwningPlayer()->GetHUD<ADefaultHUD>();
    hud->OnEscapeAction();
}
