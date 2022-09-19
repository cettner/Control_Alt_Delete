// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectionCamera.h"

#include "../RTSPlayerController.h"



// Called to bind functionality to input
void ARTSSelectionCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ARTSSelectionCamera::SelectPressed);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &ARTSSelectionCamera::SelectReleased);
	InputComponent->BindAction("RightMouse", IE_Pressed, this, &ARTSSelectionCamera::MoveSelected);
	InputComponent->BindAction("LeftMouse", IE_DoubleClick, this, &ARTSSelectionCamera::SelectDoublePressed);
}

void  ARTSSelectionCamera::SelectPressed()
{
		/*Capture where the mouse position, to be used later if we box select*/
		const ARTSPlayerController * pc = GetController<ARTSPlayerController>();
		pc->GetMousePosition(InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y);

		/*If we Have Units from a Previous Selection, Deselect them*/
		ClearSelection();

		/*Try for a box select, other selection types will interrupt and clear this if they occur*/
		CurrentSelectionType = ESelectionType::BoxSelect;
		GetWorldTimerManager().SetTimer(SelectionTimerHandle, this, &ARTSSelectionCamera::ProcessSelection, BoxSelectDelayBuffer);
}

void ARTSSelectionCamera::SelectReleased()
{
	FTimerManager& timemanager = GetWorldTimerManager();

	/*We've released before box select could become enabled*/
	if(timemanager.IsTimerActive(SelectionTimerHandle))
	{
		CurrentSelectionType = ESelectionType::SingleSelect;
	}

	ProcessSelection();
}

void ARTSSelectionCamera::SelectDoublePressed()
{
	CurrentSelectionType = ESelectionType::DoubleSelect;
	ProcessSelection();
}
	
void ARTSSelectionCamera::MoveSelected()
{
	ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetController());
	if (SelectedUnits.Num() > 0  && PC)
	{
		FHitResult Hit;
		PC->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);
		//PC->MoveMinions(PC, SelectedUnits, Hit);
	}
}

void ARTSSelectionCamera::ProcessSelection()
{
	/*Stop the timer if we're running*/
	GetWorldTimerManager().ClearTimer(SelectionTimerHandle);

	switch (CurrentSelectionType)
	{
	case ESelectionType::BoxSelect:
		HandleBoxSelect();
		break;
	
	case ESelectionType::DoubleSelect:
		HandleDoubleSelect();
		CurrentSelectionType = ESelectionType::NotSelecting;
		break;

	case ESelectionType::SingleSelect:
		HandleSingleSelect();
		CurrentSelectionType = ESelectionType::NotSelecting;
		break;

	default:
		//Do nothing
		break;
	}

	/*BroadCast the Results of our Selection*/
	SelectionUpdateDelegate.ExecuteIfBound(SelectedUnits);
}

void ARTSSelectionCamera::AddObjectToSelection(IRTSObjectInterface* InObj)
{
	if (IRTSObjectInterface::IsRTSObjectValid(InObj))
	{
		InObj->SetSelected();
		TScriptInterface<IRTSObjectInterface> obj = TScriptInterface<IRTSObjectInterface>(CastChecked<UObject>(InObj));
		SelectedUnits.AddUnique(obj);
	}

}

void ARTSSelectionCamera::AddObjectToSelection(TArray<IRTSObjectInterface*> InObjs)
{
	for (int i = 0; i < InObjs.Num(); i++)
	{
		AddObjectToSelection(InObjs[i]);
	}
}

int32 ARTSSelectionCamera::ClearSelection()
{ 
	const int32 retval = SelectedUnits.Num();
	for (int i = 0; i < SelectedUnits.Num(); i++)
	{
		if (IsValid(SelectedUnits[i].GetObject()))
		{
			SelectedUnits[i]->SetDeselected();
		}
	}
	SelectedUnits.Reset();

	/*BroadCast the Results of our Selection*/
	if (SelectionUpdateDelegate.IsBound())
	{
		SelectionUpdateDelegate.ExecuteIfBound(SelectedUnits);
	}

	return retval;
}

void ARTSSelectionCamera::HandleBoxSelect()
{
	const APlayerController* pc = GetController<APlayerController>();
	ARTSHUD* HudPtr = pc->GetHUD<ARTSHUD>();
	if (HudPtr->IsSelectionInProgress())
	{
		BoxSelectionEnd();
		CurrentSelectionType = ESelectionType::NotSelecting;
	}
	else
	{
		BoxSelectionStart();
	}
}

void ARTSSelectionCamera::BoxSelectionStart()
{
	const ARTSPlayerController* PC = Cast<ARTSPlayerController>(GetController());
	ARTSHUD * HudPtr = PC->GetHUD<ARTSHUD>();
	HudPtr->StartRTSSelection(InitialMouseScreenPosition);
}

void ARTSSelectionCamera::BoxSelectionEnd()
{
	const APlayerController* pc = GetController<APlayerController>();
	ARTSHUD* HudPtr = pc->GetHUD<ARTSHUD>();
	TArray<IRTSObjectInterface *> selectedobjects = HudPtr->FinishRTSSelection();
	AddObjectToSelection(selectedobjects);
}

void ARTSSelectionCamera::HandleDoubleSelect()
{
	/*Perform Trace*/
	const ARTSPlayerController* pc = GetController<ARTSPlayerController>();
	FHitResult Hit;
	pc->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);

	IRTSObjectInterface* hitobj = Cast<IRTSObjectInterface>(Hit.GetActor());
	if (IRTSObjectInterface::IsRTSObjectValid(hitobj))
	{
		/*Get All Units of the same class*/
		UClass* classtoselect = Hit.GetActor()->GetClass();
		const TArray<IRTSObjectInterface *> orderableunits = pc->GetOrderableUnits(classtoselect);

		for (int i = 0; i < orderableunits.Num(); i++)
		{
			/*Get only the units currently rendered on the screen*/
			const AActor* actorportion = CastChecked<AActor>(orderableunits[i]);
			if (actorportion->GetLastRenderTime() > .01f)
			{
				AddObjectToSelection(orderableunits[i]);
			}
		}
	}
}

void ARTSSelectionCamera::HandleSingleSelect()
{
	/*Todo: currnelty not an issue, but single selects are not ignored when perfomring a double select, so the first select is "wasted"*/
	/*Perform Trace*/
	const APlayerController* pc = GetController<APlayerController>();
	FHitResult Hit;
	pc->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);

	IRTSObjectInterface* hitobj = Cast<IRTSObjectInterface>(Hit.GetActor());
	if (hitobj != nullptr)
	{
		AddObjectToSelection(hitobj);
	}
}
