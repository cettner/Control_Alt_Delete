// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSAIController.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"


const FName ARTSAIController::AIMessage_Finished = TEXT("Task Complete");

ARTSAIController::ARTSAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	PerceptionComp = CreateDefaultSubobject<URTSAIPerceptionComponent>(TEXT("PerceptionComp"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	FlockPathingComp = CreateDefaultSubobject<UFlockPathFollowingComponent>(TEXT("FlockPathFollowing Component"));
	

	SetPerceptionComponent(*PerceptionComp);

	SightConfig->SightRadius = DefaultPerceptionConfig.SightRadius;
	SightConfig->LoseSightRadius = DefaultPerceptionConfig.LoseSightRadius;
	SightConfig->DetectionByAffiliation = DefaultPerceptionConfig.SightAffiliation;
	SightConfig->PeripheralVisionAngleDegrees = DefaultPerceptionConfig.PeripheralVision;

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), false);

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARTSAIController::OnTargetPerceptionUpdated);

	AIRequestId = 1U;
}

void ARTSAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	ARTSMinion *Minion = Cast<ARTSMinion>(InPawn);

	if (Minion != NULL && Minion->GetBehavior() != NULL)
	{
		BlackboardComp->InitializeBlackboard(*Minion->GetBehavior()->BlackboardAsset);
		BehaviorComp->StartTree(*Minion->GetBehavior());
		
		if (!ConfigureRTSPerception(Minion))
		{
			UE_LOG(LogTemp, Warning, TEXT("[ARTSAIController::OnPossess] Failed to configure Perception"));
		}
	}
}

bool ARTSAIController::ConfigureRTSPerception(ARTSMinion* Minion)
{
	if (PerceptionComponent == nullptr) return false;
	FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());  

	if (!Id.IsValid()) return false;

	UAISenseConfig_Sight * sightperceptionconfig = Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(Id));
	if (sightperceptionconfig == nullptr) return false;

	FRTSAIPerceptionConfig config = Minion->GetAIConfig();
	sightperceptionconfig->SightRadius = config.SightRadius;
	sightperceptionconfig->LoseSightRadius = config.LoseSightRadius;
	sightperceptionconfig->DetectionByAffiliation = config.SightAffiliation;
	sightperceptionconfig->PeripheralVisionAngleDegrees = config.PeripheralVision;
	PerceptionComponent->RequestStimuliListenerUpdate();

	return true;
}

ETeamAttitude::Type ARTSAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IRTSObjectInterface* threat = Cast<IRTSObjectInterface>(&Other);
	const IRTSObjectInterface* myminion = GetPawn<IRTSObjectInterface>();

	if (threat && myminion && myminion->GetTeam() != threat->GetTeam())
	{
		return(ETeamAttitude::Hostile);
	}

	return ETeamAttitude::Neutral;
}

void ARTSAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetPathFollowingComponent(FlockPathingComp);
	FlockPathingComp->OnRequestFinished.AddUObject(this, &AAIController::OnMoveCompleted);
}

void ARTSAIController::SetTarget(AActor * newtarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(Target, newtarget);
	}
}

AActor * ARTSAIController::GetTarget()
{
	return(Cast<AActor>(BlackboardComp->GetValueAsObject(Target)));
}

void ARTSAIController::ClearTarget()
{
	BlackboardComp->ClearValue(Target);
}

void ARTSAIController::ReleaseAssets()
{
}

void ARTSAIController::SetCommander(ACommander * Commander)
{
	BlackboardComp->SetValueAsObject("OwningCommander", Commander);
}

void ARTSAIController::SendAIMessage(const FName AIMessage, FAIMessage::EStatus Status)
{
	FAIMessage Msg(AIMessage, this, AIRequestId, Status);
	FAIMessage::Send(this, Msg);
	StoreAIRequestId();
}

void ARTSAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
}

void ARTSAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

}

ACommander * ARTSAIController::GetCommander()
{
	return(Cast<ACommander>(BlackboardComp->GetValueAsObject("OwningCommander")));
}

void ARTSAIController::ClearCommander()
{
	BlackboardComp->ClearValue("OwningCommander");
}

