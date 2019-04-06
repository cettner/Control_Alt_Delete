// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSAIController.h"
#include "BuilderAIController.generated.h"

/**
 * 
 */


UCLASS()
class RTS_PROJECT_API ABuilderAIController : public ARTSAIController
{
	GENERATED_BODY()


	static uint32 NextRequestId;
	FAIRequestID MineRequestId;

//	FORCEINLINE void StoreMineRequestId() { MineRequestId = NextRequestId++; }

public:
	//FORCEINLINE FAIRequestID GetMineRequestId() const { return MineRequestId; }
	void SendMineUpdateMessage();
	static const FName AIMessage_Mine_Finished;
};