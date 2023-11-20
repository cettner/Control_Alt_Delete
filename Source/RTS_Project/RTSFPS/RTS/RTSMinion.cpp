// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultPlayerState.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"



ARTSMinion::ARTSMinion()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	/*SelectionRings Shouldnt appear on mesh*/
	GetMesh()->bReceivesDecals = false;
	/*Execute Montages even when not visible by the server, as socket locations are always relevant*/
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	//AI recives Access on Spawn
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ARTSAIController::StaticClass();

	// No Ticking on Minions
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Selection = CreateDefaultSubobject<UDecalSelectionComponent>(TEXT("Selection"));
	Selection->SetDetection(GetCapsuleComponent());
	Selection->SetupAttachment(RootComponent);
	SetDeselected();

	ResourceComp = CreateDefaultSubobject<UResourceGathererComponent>(TEXT("ResourceComp"));
	ResourceComp->SetIsReplicated(true);

	DeathComp = CreateDefaultSubobject<UDeathComponent>(TEXT("DeathComp"));
	DeathComp->OnDeathStart.BindUObject(this, &ARTSMinion::OnDeath);
	DeathComp->SetIsReplicated(true);

	/*Radial from forward vector ie: 180 == abs(0 +- 90)*/
	AIConfig.PeripheralVision = 180.0;
	AIConfig.SightRadius = 1500;
	AIConfig.LoseSightRadius = 2000.0;

	FAISenseAffiliationFilter Affiliations = FAISenseAffiliationFilter();
	Affiliations.bDetectEnemies = false;
	Affiliations.bDetectFriendlies = false;
	Affiliations.bDetectNeutrals = false;

	AIConfig.SightAffiliation = Affiliations;
}

TSubclassOf<UResource> ARTSMinion::GetResourceForDamageEvent(const TSubclassOf<UDamageType>& InDamageType) const
{
	TSubclassOf<UResource> retval = DefaultHealthClass;
	if (const TSubclassOf<UResource> * found = DamageMapping.Find(InDamageType))
	{
		retval = *found;
	}
	return retval;
}

void ARTSMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FOnResourceValueChangedDelegate& changedelegate = BindResourceValueChangedEvent(DefaultHealthClass);
	changedelegate.AddUObject(this, &ARTSMinion::OnHealthResourceChanged);

	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
		if (PerceptionSystem)
		{
			PerceptionSystem->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
		}
	}
}

void ARTSMinion::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterRTSObject();
}

bool ARTSMinion::StartAttack(const int32 InAttackID)
{
	return false;
}

bool ARTSMinion::StopAttack(const bool InForceStop)
{
	return true;
}

int32 ARTSMinion::GetAttackIndexForTarget(const AActor* InToAttack) const
{
	return CANT_ATTACK_INDEX;
}

bool ARTSMinion::IsAlive() const
{
	const bool retval = DeathComp && DeathComp->IsAlive();
	return (retval);
}

bool ARTSMinion::IsBoxSelectable() const
{
	return bIsBoxSelectable;
}

UTexture* ARTSMinion::GetThumbnail(const UUserWidget* InDisplayContext) const
{
	return Thumbnail;
}

FName ARTSMinion::GetUnitName() const
{
	return MinionName;
}

const TSubclassOf<URTSTargetedOrder> ARTSMinion::GetDefaultOrderClass(const FHitResult& InHitContext) const
{
	return MoveOrderClass;
}

void ARTSMinion::IssueOrder(AController* Issuer, const FHitResult& InHitContext, URTSOrder* InOrder, const bool InbIsQueuedOrder)
{
	ARTSAIController* rtscontrol = GetController<ARTSAIController>();
	rtscontrol->EnqueueOrder(InOrder, InbIsQueuedOrder);
}

float ARTSMinion::GetMinionStrayDistance() const
{
	return StrayDistance;
}

void ARTSMinion::RegisterRTSObject()
{
	UWorld * world = GetWorld();
	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	static FDelegateHandle skiphandle = FDelegateHandle();

	if (IsValid(gs))
	{
		gs->RegisterRTSObject(this);
		skiphandle.Reset();
	}
	else
	{
		skiphandle = world->GameStateSetEvent.AddUObject(this, &ARTSMinion::OnNotifyReadyforRegistration);
	}
}

void ARTSMinion::UnRegisterRTSObject()
{
	const UWorld* world = GetWorld();
	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	gs->UnRegisterRTSObject(this);
}

void ARTSMinion::OnDeath()
{
	if (HasAuthority())
	{
		/*Unregister Minion to be percieved from AI perception*/
		UWorld* World = GetWorld();
		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
		PerceptionSystem->UnregisterSource(*this);

		/*Notify the Gamestate We Died*/
		ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();
		GS->OnUnitDeath(this);
	}

	/*Todo: make this an event / delegate broadcast to the hud rather than calling it directly*/
	if (GetNetMode() != NM_DedicatedServer && IsSelected())
	{
		ARTSHUD* hud = GetWorld()->GetFirstPlayerController()->GetHUD<ARTSHUD>();
		hud->ForceRemoveSelection(this);
	}

	UnRegisterRTSObject();
}

void ARTSMinion::OnUpgradeChanged(const TSubclassOf<UUpgrade> InUpgradeclass, const int32 InOldRank, const int32 InNewRank)
{
	InstallUpgrade(InUpgradeclass, InOldRank, InNewRank);
}

bool ARTSMinion::InstallUpgrade(const TSubclassOf<UUpgrade> Upgradeclass, const int32 OldRank, const int32 NewRank)
{
	return false;
}

UObject* ARTSMinion::GetUpgradeSubObject(const TSubclassOf<UUpgrade>& InUpgradeclass) const
{
	return nullptr;
}

void ARTSMinion::OnHealthResourceChanged(const TSubclassOf<UResource> InResourceClass, const uint32 InOldValue, const uint32 InNewValue, TScriptInterface<IResourceGatherer> InGatherer)
{
	if (InResourceClass == DefaultHealthClass && InNewValue == 0U)
	{
		DeathComp->Die();
	}
}

void ARTSMinion::OnNotifyReadyforRegistration(AGameStateBase* GameState)
{
	RegisterRTSObject();
}

bool ARTSMinion::IsEnemy(const AActor* FriendOrFoe) const
{
	bool Enemy = false;
	if (!IsValid(FriendOrFoe)) return false;

	const IRTSObjectInterface* rtsobj = Cast<IRTSObjectInterface>(FriendOrFoe);

	if (rtsobj && rtsobj->GetTeam() != this->GetTeam() && rtsobj->GetTeam() >= 0)
	{
		Enemy = true;
	}

	return(Enemy);
}

void ARTSMinion::SetSelected()
{
	bool isenemyselection = IsLocalEnemy();
	if (!isenemyselection)
	{
		Selection->SetHiddenInGame(false);
	}
	else
	{
		/*Bright Red Color to indicate selection*/
		Selection->SetSelectionColor(FLinearColor::Red);
	}
}

void ARTSMinion::SetDeselected()
{
	bool isenemyselection = IsLocalEnemy();
	if (!isenemyselection)
	{
		Selection->SetHiddenInGame(true);
	}
	else
	{
		FLinearColor enemycolor = FLinearColor::Red;
		enemycolor.A = .25;
		Selection->SetSelectionColor(enemycolor);
	}
}

void ARTSMinion::SetTeamColors(FLinearColor TeamColor)
{
	if (Selection)
	{
		Selection->SetSelectionColor(TeamColor);
	}
}

IRTSObjectInterface * ARTSMinion::GetLeadRTSObject()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ARTSAIController * AIC = GetController<ARTSAIController>();
		ACommander * commander = AIC->GetLeadRTSObject();

		return(commander);
	}
	else
	{
		return(Cmdr);
	}

}

void ARTSMinion::ClearCommander()
{
	ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
	AIC->ClearCommander();
	Cmdr = nullptr;
}

void ARTSMinion::SetCommander(ACommander * Commander)
{
	ARTSAIController * AIC = GetController<ARTSAIController>();
	AIC->SetCommander(Commander);
	Cmdr = Commander;
}

void ARTSMinion::SetTeam(int team_id)
{
	if (HasAuthority())
	{
		TeamID = team_id;
		if (GetNetMode() != NM_DedicatedServer)
		{
			OnRep_TeamID();
		}
	}
}

int ARTSMinion::GetTeam() const
{
	return TeamID;
}

void ARTSMinion::OnLocalPlayerTeamChange(int InLocalTeamID)
{
	OnRep_TeamID();
}

bool ARTSMinion::IsLocalEnemy() const
{
	return bIsLocalEnemy;
}

void ARTSMinion::OnRep_TeamID()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	
	const ADefaultPlayerController * PC = World->GetFirstPlayerController<ADefaultPlayerController>();
	if (PC == nullptr) return;

	/*We are an enemy to to the local player*/
	const ADefaultPlayerState* PS = PC->GetPlayerState<ADefaultPlayerState>();
	if (PS && PS->GetTeamID() != TeamID)
	{
		/*Local Enemy Setup*/
		bIsLocalEnemy = true;
		FLinearColor enemycolor = FLinearColor::Red;
		enemycolor.A = .25;
		SetTeamColors(enemycolor);
		Selection->SetHiddenInGame(false);
	}
	else if(PS && PS->GetTeamID() == TeamID)
	{
		/*Enable box selection*/
		bIsLocalEnemy = false;
		bIsBoxSelectable = true;
		SetDeselected();
	}
}

UBehaviorTree* ARTSMinion::GetBehavior()
{
	return(RTSBehavior);
}

FRTSAIPerceptionConfig ARTSMinion::GetAIConfig() const
{
	return AIConfig;
}

bool ARTSMinion::UsesAISenses() const
{
	return AISenseClasses.Num() > 0;
}

TArray<TSubclassOf<UAISense>> ARTSMinion::GetAISenses() const
{
	return AISenseClasses;
}

float ARTSMinion::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
 	float ProcessedDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	const TSubclassOf<UResource> healthclass = GetResourceForDamageEvent(DamageEvent.DamageTypeClass);
	const uint32 damagefinal = StaticCast<uint32>(ProcessedDamage);
	RemoveResource(healthclass, damagefinal);
	return (ProcessedDamage);
}

void ARTSMinion::AddResource(TSubclassOf<UResource> InResourceType, int InAmount)
{
	ResourceComp->AddResource(InResourceType, InAmount);
}

bool ARTSMinion::RemoveResource(const TSubclassOf<UResource> InResourceType, int InAmount)
{
	return 	ResourceComp->RemoveResource(InResourceType, InAmount);
}

FReplicationResourceMap ARTSMinion::GetAllHeldResources() const
{
	return ResourceComp->GetAllHeldResources();
}

uint32 ARTSMinion::GetCurrentWeight() const
{
	return ResourceComp->GetCurrentWeight();
}

uint32 ARTSMinion::GetMaxWeight() const
{
	return ResourceComp->GetMaxWeight();
}

uint32 ARTSMinion::GetResourceMaximum(const TSubclassOf<UResource> ResourceClass) const
{
	return ResourceComp->GetResourceMaximum(ResourceClass);
}

uint32 ARTSMinion::GetResourceMinimum(const TSubclassOf<UResource> ResourceClass) const
{
	return ResourceComp->GetResourceMinimum(ResourceClass);
}

FOnResourceValueChangedDelegate& ARTSMinion::BindResourceValueChangedEvent(const TSubclassOf<UResource> InResourceType)
{
	return ResourceComp->BindResourceValueChangedEvent(InResourceType);
}

void ARTSMinion::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARTSMinion, Cmdr);
	DOREPLIFETIME(ARTSMinion, TeamID)
}