// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../AI/RTSAIController.h"
#include "BuilderAIController.generated.h"

/**
 * 
 */


UCLASS()
class RTS_PROJECT_API ABuilderAIController : public ARTSAIController
{
	GENERATED_BODY()

private:
	FAIRequestID MineRequestId;

	FORCEINLINE void StoreMineRequestId() { MineRequestId = MineRequestId + 1; }

protected:
	ABuilderAIController();

public:
	FORCEINLINE FAIRequestID GetMineRequestId() const { return MineRequestId; }
	void SendMineUpdateMessage();
	static const FName AIMessage_Mine_Finished;

public:
//	virtual void ReleaseAssets() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	FName LastMinedLocation = "LastMinedLocation";
};