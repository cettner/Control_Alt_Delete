// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSObjectInterface : public UInterface
{
	GENERATED_BODY()
};


class RTS_PROJECT_API IRTSObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetSelected();
	virtual void SetDeselected();
	virtual bool IsBoxSelectable() const;

	virtual int GetTeam() const;
	virtual void SetTeam(int teamid);
	virtual void SetTeamColors(FLinearColor TeamColor);
	/*Called when the local players team has been set either from initializtion, replication, or runtime*/
	virtual void OnLocalPlayerTeamChange(int InLocalTeamID);

	virtual bool IsAlive() const;
	static bool IsRTSObjectValid(const IRTSObjectInterface * InRTSObject);

	virtual void SetTarget(AActor* InNewTarget);
	virtual void SetTargetLocation(FVector InTargetLocation);
	virtual AActor* GetTarget();
	virtual void ClearTarget();

	virtual IRTSObjectInterface* GetLeadRTSObject();

	template<class T>
	T* GetLeadRTSObject() { return Cast<T>(GetLeadRTSObject()); }



protected:
	virtual void RegisterRTSObject() = 0;
	virtual void UnRegisterRTSObject() = 0;
};
