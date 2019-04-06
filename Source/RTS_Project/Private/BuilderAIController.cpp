// Fill out your copyright notice in the Description page of Project Settings.

#include "BuilderAIController.h"
#include"Runtime/AIModule/Classes/BrainComponent.h"


const FName ABuilderAIController::AIMessage_Mine_Finished = TEXT("MineComplete");

void ABuilderAIController::SendMineUpdateMessage()
{
	FAIMessage Msg(AIMessage_Mine_Finished, this, MineRequestId, FAIMessage::Success);
	FAIMessage::Send(this, Msg);
}
