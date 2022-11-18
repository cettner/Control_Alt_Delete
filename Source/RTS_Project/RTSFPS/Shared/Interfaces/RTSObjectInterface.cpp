// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSObjectInterface.h"

// Add default functionality here for any IRTSObjectInterface functions that are not pure virtual.

void IRTSObjectInterface::SetSelected()
{
}

void IRTSObjectInterface::SetDeselected()
{
}

bool IRTSObjectInterface::IsBoxSelectable() const
{
	return false;
}

int IRTSObjectInterface::GetTeam() const
{
	return -1;
}

void IRTSObjectInterface::SetTeam(int teamid)
{
}

void IRTSObjectInterface::SetTeamColors(FLinearColor TeamColor)
{
}

void IRTSObjectInterface::OnLocalPlayerTeamChange(int InLocalTeamID)
{
}

bool IRTSObjectInterface::IsAlive() const
{
	return false;
}

bool IRTSObjectInterface::IsRTSObjectValid(const IRTSObjectInterface* InRTSObject)
{
	return IsValid(Cast<UObject>(InRTSObject));
}

bool IRTSObjectInterface::IsOrderablebyController(const AController* InController) const
{
	return false;
}

bool IRTSObjectInterface::CanCompleteOrder(const URTSOrder* InOrder) const
{
	const TArray<const URTSOrder*> availableorders = GetAvailableOrders();
	const bool retval = availableorders.Find(InOrder) > INDEX_NONE;
	return retval;
}

const TArray<const URTSOrder*> IRTSObjectInterface::GetAvailableOrders() const
{
	return TArray<const URTSOrder*>();
}

const TSubclassOf<URTSTargetedOrder> IRTSObjectInterface::GetDefaultOrderClass(const FHitResult& InHitContext) const
{
	return nullptr;
}

const URTSOrder* IRTSObjectInterface::GetCurrentOrder() const
{
	return nullptr;
}

TArray<const URTSOrder*> IRTSObjectInterface::GetAllIssuedOrders() const
{
	return TArray<const URTSOrder*>();
}

TArray<const URTSProperty*> IRTSObjectInterface::GetRTSProperties(bool bIncludeNestedProperties) const
{
	return TArray<const URTSProperty *>();
}

bool IRTSObjectInterface::ContainsProperty(const URTSProperty* InPropCheck) const
{
	checkf(InPropCheck, TEXT("IRTSObjectInterface::ContainsProperty InProp was null"));
	const TArray<const URTSProperty*> props = GetRTSProperties(true);

	bool retval = false;

	for (int i = 0; i <  props.Num(); i ++)
	{
		retval =  props[i]->GetClass() == InPropCheck->GetClass();

		if (retval == true)
		{
			break;
		}
	}
	
	return retval;
}

UTexture* IRTSObjectInterface::GetThumbnail(const UUserWidget* InDisplayContext) const
{
	return nullptr;
}

FName IRTSObjectInterface::GetUnitName() const
{
	return FName();
}

void IRTSObjectInterface::IssueOrder(AController * InIssuer, const FHitResult& InHitContext, const URTSOrder* InOrderClass, const bool InbIsQueuedOrder)
{

}

IRTSObjectInterface* IRTSObjectInterface::GetLeadRTSObject()
{
	return nullptr;
}
