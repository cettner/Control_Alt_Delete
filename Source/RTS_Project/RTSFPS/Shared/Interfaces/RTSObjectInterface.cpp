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

void IRTSObjectInterface::SetTarget(AActor* InNewTarget)
{
}

void IRTSObjectInterface::SetTargetLocation(FVector InTargetLocation)
{
}

AActor* IRTSObjectInterface::GetTarget()
{
	return nullptr;
}

void IRTSObjectInterface::ClearTarget()
{
}

bool IRTSObjectInterface::IsOrderablebyController(const AController* InController) const
{
	return false;
}

bool IRTSObjectInterface::CanCompleteOrder(const TSubclassOf<URTSOrder> InOrderClass) const
{
	const TArray<TSubclassOf<URTSOrder>> availableorders = GetAvailableOrders();
	const bool retval = availableorders.Find(InOrderClass) > INDEX_NONE;
	return retval;
}

const TArray<TSubclassOf<URTSOrder>> IRTSObjectInterface::GetAvailableOrders() const
{
	return TArray<TSubclassOf<URTSOrder>>();
}

TSubclassOf<URTSOrder> IRTSObjectInterface::GetDefaultOrder(const FHitResult& InHitContext) const
{
	return nullptr;
}

TSubclassOf<URTSOrder> IRTSObjectInterface::GetCurrentOrder() const
{
	return TSubclassOf<URTSOrder>();
}

TArray<TSubclassOf<URTSOrder>> IRTSObjectInterface::GetAllIssuedOrders() const
{
	return TArray<TSubclassOf<URTSOrder>>();
}

TArray<TSubclassOf<URTSProperty>> IRTSObjectInterface::GetRTSProperties() const
{
	return TArray<TSubclassOf<URTSProperty>>();
}

bool IRTSObjectInterface::ContainsProperty(TSubclassOf<URTSProperty> InPropCheck) const
{
	return false;
}

void IRTSObjectInterface::IssueOrder(const FHitResult& InHitContext, const TSubclassOf<URTSOrder> InOrderClass, const bool InbIsQueuedOrder)
{

}

IRTSObjectInterface* IRTSObjectInterface::GetLeadRTSObject()
{
	return nullptr;
}
