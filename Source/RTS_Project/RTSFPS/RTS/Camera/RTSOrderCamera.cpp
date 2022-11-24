// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrderCamera.h"
#include "../RTSPlayerController.h"

void ARTSOrderCamera::LoadOrder(URTSTargetedOrder* InOrder)
{
    if (IsValid(EnqueuedOrder))
    {
        ClearOrder();
    }
    
    EnqueuedOrder = InOrder;
}

URTSTargetedOrder* ARTSOrderCamera::GetOrder() const
{
    return EnqueuedOrder;
}

void ARTSOrderCamera::ClearOrder()
{
    EnqueuedOrder = nullptr;
}

void ARTSOrderCamera::OrderSelected()
{
    if (SelectedUnits.Num() > 0)
    {
        ARTSPlayerController* pc = GetController<ARTSPlayerController>();
        FHitResult hitresult = FHitResult();
        pc->GetHitResultUnderCursor(SELECTION_CHANNEL, false, hitresult);
        pc->IssueDefaultOrder(GetSelectedUnits(), hitresult, false);
    }
}

void ARTSOrderCamera::SelectPressed()
{
    if (URTSTargetedOrder * enquedorder = GetOrder())
    {
        ARTSPlayerController* pc = GetController<ARTSPlayerController>();
        FHitResult hitresult = FHitResult();
        pc->GetHitResultUnderCursor(SELECTION_CHANNEL, false, hitresult);
        TArray <TScriptInterface<IRTSObjectInterface>> unitstoissue = enquedorder->GetBestMinionsForOrder(GetSelectedUnits(), hitresult);

        pc->IssueOrder(unitstoissue, enquedorder, hitresult, false);
        ClearOrder();
    }
    else
    {
        Super::SelectPressed();
    }
}

// Called to bind functionality to input
void ARTSOrderCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    InputComponent->BindAction("RightMouse", IE_Pressed, this, &ARTSOrderCamera::OrderSelected);
}