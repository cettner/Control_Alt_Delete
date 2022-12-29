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
    const TArray <TScriptInterface<IRTSObjectInterface>> orderableunits = GetOrderableUnitsFromSelection();
    if (orderableunits.Num() > 0)
    {
        ARTSPlayerController* pc = GetController<ARTSPlayerController>();
        FHitResult hitresult = FHitResult();
        pc->GetHitResultUnderCursor(SELECTION_CHANNEL, false, hitresult);
        pc->IssueDefaultOrder(orderableunits, hitresult, false);
    }
}

TArray<TScriptInterface<IRTSObjectInterface>> ARTSOrderCamera::GetOrderableUnitsFromSelection() const
{
    const ARTSPlayerController* pc = GetController<ARTSPlayerController>();
    TArray<TScriptInterface<IRTSObjectInterface>> retval = TArray<TScriptInterface<IRTSObjectInterface>>();
    const TArray<TScriptInterface<IRTSObjectInterface>> currentselection = GetSelectedUnits();

    for (int i = 0; i < currentselection.Num(); i++)
    {
        if (pc->IsUnitOrderable(CastChecked<IRTSObjectInterface>(currentselection[i].GetObject())))
        {
            retval.Emplace(currentselection[i]);
        }
    }

    return retval;
}

void ARTSOrderCamera::SelectPressed()
{
    TArray <TScriptInterface<IRTSObjectInterface>> orderableunits = GetOrderableUnitsFromSelection();

    if (URTSTargetedOrder * enquedorder = GetOrder())
    {
        ARTSPlayerController* pc = GetController<ARTSPlayerController>();
        FHitResult hitresult = FHitResult();
        pc->GetHitResultUnderCursor(SELECTION_CHANNEL, false, hitresult);
        TArray <TScriptInterface<IRTSObjectInterface>> unitstoissue = enquedorder->GetBestMinionsForOrder(orderableunits, hitresult);

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