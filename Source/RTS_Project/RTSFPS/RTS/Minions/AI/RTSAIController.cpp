// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSAIController.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/Navigation/FlowFieldFollowingComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


const FName ARTSAIController::AIMessageOrderRequest = TEXT("Task Request");
const FName ARTSAIController::AIMessageAbortRequest = TEXT("Abort Request");
const FName ARTSAIController::AIGuardLocationKey = TEXT("GuardLocation");
const FName ARTSAIController::AICurrentOrderKey = TEXT("Order");


ARTSAIController::ARTSAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	PerceptionComp = CreateDefaultSubobject<URTSAIPerceptionComponent>(TEXT("PerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	//UFlowFieldFollowingComponent* pathfollowingcomp = CreateDefaultSubobject<UFlowFieldFollowingComponent>(TEXT("FlowFieldFollowingComp"));

	SetPerceptionComponent(*PerceptionComp);
	//SetPathFollowingComponent(pathfollowingcomp);

	SightConfig->SightRadius = DefaultPerceptionConfig.SightRadius;
	SightConfig->LoseSightRadius = DefaultPerceptionConfig.LoseSightRadius;
	SightConfig->DetectionByAffiliation = DefaultPerceptionConfig.SightAffiliation;
	SightConfig->PeripheralVisionAngleDegrees = DefaultPerceptionConfig.PeripheralVision;
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARTSAIController::OnTargetPerceptionUpdated);

	AIRequestId = 1U;
	AIAbortID = 1U;
}

void ARTSAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	ARTSMinion *Minion = Cast<ARTSMinion>(InPawn);

	if (Minion != NULL && Minion->GetBehavior() != NULL)
	{
		BlackboardComp->InitializeBlackboard(*Minion->GetBehavior()->BlackboardAsset);
		BehaviorComp->StartTree(*Minion->GetBehavior());

		/*Set its initial location for guard / idle logic in the BT, it will be updated later as it performs orders*/
		BlackboardComp->SetValueAsVector(AIGuardLocationKey, Minion->GetActorLocation());

		if (Minion->UsesAISenses() && !ConfigureRTSPerception(Minion))
		{
			UE_LOG(LogTemp, Warning, TEXT("[ARTSAIController::OnPossess] Failed to configure Perception"));
		}
	}
}

bool ARTSAIController::ConfigureRTSPerception(ARTSMinion* Minion)
{
	bool retval = true;
	TArray<TSubclassOf<UAISense>> senses = Minion->GetAISenses();

	for (int i = 0; i < senses.Num(); i++)
	{
		FAISenseID Id = UAISense::GetSenseID(senses[i]);
		UAISenseConfig* senseconfig = PerceptionComponent->GetSenseConfig(Id);
		if (ConfigureAISense(Minion, senseconfig))
		{
			PerceptionComponent->SetSenseEnabled(senses[i], true);
			PerceptionComponent->RequestStimuliListenerUpdate();
		}
		else
		{
			retval = false;
		}
	}

	return retval;
}

bool ARTSAIController::ConfigureAISense(ARTSMinion* InMinion, UAISenseConfig* InSenseConfig)
{
	bool retval = false;
	if (UAISenseConfig_Sight* sightconfig = Cast<UAISenseConfig_Sight>(InSenseConfig))
	{
		FRTSAIPerceptionConfig config = InMinion->GetAIConfig();
		sightconfig->SightRadius = config.SightRadius;
		sightconfig->LoseSightRadius = config.LoseSightRadius;
		sightconfig->DetectionByAffiliation = config.SightAffiliation;
		sightconfig->PeripheralVisionAngleDegrees = config.PeripheralVision;
		sightconfig->SetMaxAge(.01f);

		retval = true;
	}

	return retval;
}

ETeamAttitude::Type ARTSAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IRTSObjectInterface* threat = Cast<IRTSObjectInterface>(&Other);
	const IRTSObjectInterface* myminion = GetPawn<IRTSObjectInterface>();
	ETeamAttitude::Type retval = ETeamAttitude::Neutral;

	if (threat && myminion)
	{
		const int thierteam = threat->GetTeam();
		const int myteam = myminion->GetTeam();

		if (thierteam == NEUTRAL_TEAM_INDEX)
		{

		}
		else if (thierteam != myteam)
		{
			retval = ETeamAttitude::Hostile;
		}
		else
		{
			retval = ETeamAttitude::Friendly;
		}
	}

	return(retval);
}

FPathFollowingRequestResult ARTSAIController::MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath)
{
	FPathFollowingRequestResult ResultData = Super::MoveTo(MoveRequest, OutPath);
	/*
	FPathFollowingRequestResult ResultData;
	ResultData.Code = EPathFollowingRequestResult::Failed;

	const bool bAlreadyAtGoal = GetPathFollowingComponent()->HasReached(MoveRequest);

	if (bAlreadyAtGoal)
	{
		ResultData.MoveId = GetPathFollowingComponent()->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
		ResultData.Code = EPathFollowingRequestResult::AlreadyAtGoal;
	}
	else
	{
		FVectorFieldQuery VFQuery;
		const bool bValidQuery = BuildPathfindingQuery(MoveRequest, VFQuery);

		if (bValidQuery)
		{
			FNavPathSharedPtr Path;
			FindFieldForMoveRequest(MoveRequest, VFQuery, Path);

			const FAIRequestID RequestID = Path.IsValid() ? RequestMove(MoveRequest, Path) : FAIRequestID::InvalidRequest;
			if (RequestID.IsValid())
			{
				if (OutPath)
				{
					*OutPath = Path;
					bAllowStrafe = MoveRequest.CanStrafe();
					ResultData.MoveId = RequestID;
					ResultData.Code = EPathFollowingRequestResult::RequestSuccessful;
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Starting Task %d"),RequestID));
				}
			}
		}

	}
	*/
	return ResultData;
}

void ARTSAIController::FindFieldForMoveRequest(const FAIMoveRequest& MoveRequest, FVectorFieldQuery& Query, FNavPathSharedPtr& OutPath) const
{
	UVectorFieldNavigationSystem* NavSys = FNavigationSystem::GetCurrent<UVectorFieldNavigationSystem>(GetWorld());

	if (NavSys)
	{
		FPathFindingResult PathResult = NavSys->FindVectorFieldPathSync(Query);
		if (PathResult.Result != ENavigationQueryResult::Error)
		{
			if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
			{
				PathResult.Path->EnableRecalculationOnInvalidation(true);
				OutPath = PathResult.Path;
			}
		}
	}
}

bool ARTSAIController::BuildPathfindingQuery(const FAIMoveRequest& MoveRequest, FVectorFieldQuery& OutQuery) const
{
	bool bResult = false;

	const UVectorFieldNavigationSystem* NavSys = FNavigationSystem::GetCurrent<UVectorFieldNavigationSystem>(GetWorld());
	const AGameGrid* NavData = (NavSys == nullptr) ? nullptr : Cast<AGameGrid>(NavSys->GetNavDataForProps(GetNavAgentPropertiesRef(), GetNavAgentLocation()));

	if (NavData)
	{
		FVector GoalLocation = MoveRequest.GetGoalLocation();
		if (MoveRequest.IsMoveToActorRequest())
		{
			const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveRequest.GetGoalActor());
			if (NavGoal)
			{
				const FVector Offset = NavGoal->GetMoveGoalOffset(this);
				GoalLocation = FQuatRotationTranslationMatrix(MoveRequest.GetGoalActor()->GetActorQuat(), NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
			}
			else
			{
				GoalLocation = MoveRequest.GetGoalActor()->GetActorLocation();
			}
		}

		OutQuery = FVectorFieldQuery(*this, *NavData, GetNavAgentLocation(), GoalLocation);
		OutQuery.IsDynamicGoal = MoveRequest.GetGoalActor() != nullptr;
		OutQuery.SetGoalActor(MoveRequest.GetGoalActor());

		bResult = true;
	}


	return bResult;
}

void ARTSAIController::SetCommander(ACommander * Commander)
{
	BlackboardComp->SetValueAsObject("OwningCommander", Commander);
}

UObject* ARTSAIController::GetBlackBoardKeyAsObject(const FName InKeyName) const
{
	return BlackboardComp->GetValueAsObject(InKeyName);
}

void ARTSAIController::SendAIMessage(const FName AIMessage, FAIMessage::EStatus Status, EAIMessageType MessageType)
{
	if (MessageType == EAIMessageType::Task || MessageType == EAIMessageType::Progress)
	{
		FAIMessage Msg(AIMessage, this, AIRequestId, Status);
		FAIMessage::Send(this, Msg);
		StoreAIRequestId(AIRequestId);
	}
	else
	{
		FAIMessage Msg(AIMessage, this, AIAbortID, Status);
		FAIMessage::Send(this, Msg);
		StoreAIRequestId(AIAbortID);
	}
	

}

bool ARTSAIController::IsAbortingTask() const
{
	return BehaviorComp->IsAbortPending();
}

URTSOrder* ARTSAIController::GetCurrentOrder() const
{
	return Cast<URTSOrder>(BlackboardComp->GetValueAsObject(AICurrentOrderKey));
}

URTSOrder* ARTSAIController::GetAbortingOrder() const
{
	return AbortingOrder;
}

void ARTSAIController::EnqueueOrder(URTSOrder* InOrder, bool InbIsEnquedOrder)
{
	if (!InbIsEnquedOrder)
	{
		ClearOrders();
		SetCurrentOrder(InOrder);
	}
	else if(IsValid(GetCurrentOrder()))
	{
		EnquedOrders.Enqueue(InOrder);
		NumOrders += 1;
	}
	else
	{
		SetCurrentOrder(InOrder);
	}
}

void ARTSAIController::ClearOrders()
{
	EnquedOrders.Empty();
	if (IsValid(GetCurrentOrder()))
	{
		OnOrderFinished(nullptr, false);
	}
}

void ARTSAIController::SetCurrentOrder(URTSOrder* InOrder)
{
	if (InOrder != GetCurrentOrder())
	{
		URTSOrder* nextorder = nullptr;

		if (InOrder != nullptr)
		{
			/*Make Sure the blackboard is properly loaded before we set the current order*/
			InOrder->LoadAIBlackBoard(BlackboardComp);
			BlackboardComp->SetValueAsObject(AICurrentOrderKey, InOrder);
			CurrentOrder = InOrder;
		}
		/*NullOrder, possibly due to completion of a previous order, check queue if we have one available and load it*/
		else if (InOrder == nullptr && EnquedOrders.Dequeue(nextorder))
		{
			NumOrders -= 1;
			/*Make Sure the blackboard is properly loaded before we set the current order*/
			nextorder->LoadAIBlackBoard(BlackboardComp);
			BlackboardComp->SetValueAsObject(AICurrentOrderKey, nextorder);
			CurrentOrder = nextorder;
		}
		/*Clear the Order*/
		else
		{
			BlackboardComp->ClearValue(AICurrentOrderKey);
			CurrentOrder = nullptr;
		}
	}
}

void ARTSAIController::OnOrderFinished(UBTTaskNode* InTaskNode, const bool InOrderSuccess)
{
	SetCurrentOrder(nullptr);
}

void ARTSAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
}

void ARTSAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Stimulus.WasSuccessfullySensed())
	{
		PerceptionComp->ForgetActor(Actor);
	}
}

ACommander * ARTSAIController::GetLeadRTSObject()
{
	return(Cast<ACommander>(BlackboardComp->GetValueAsObject("OwningCommander")));
}

void ARTSAIController::ClearCommander()
{
	BlackboardComp->ClearValue("OwningCommander");
}

