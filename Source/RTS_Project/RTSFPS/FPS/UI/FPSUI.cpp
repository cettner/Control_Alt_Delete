// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSUI.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h" 


UFPSUI::UFPSUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HealthBar = CreateDefaultSubobject<UProgressBar>(TEXT("Health Bar"));
	CurrentHealthText = CreateDefaultSubobject<UTextBlock>(TEXT("Current Health Text"));
	MaxHealthText = CreateDefaultSubobject<UTextBlock>(TEXT("Max Health Text"));
	TeamResourceList = CreateDefaultSubobject<UPanelWidget>(TEXT("Team Resource List"));

	ResourceWidgetClass = nullptr;

	FloatingPointOptions.MaximumFractionalDigits = 2;
	FloatingPointOptions.MinimumFractionalDigits = 0;
	FloatingPointOptions.UseGrouping = false;
	FloatingPointOptions.MaximumIntegralDigits = 324;
}

bool UFPSUI::Initialize()
{
	bool success = Super::Initialize();
	if (success == false || HealthBar == nullptr) return(false);

	/*Function is called via Synchronize properties from Tick*/
	HealthBar->PercentDelegate.BindUFunction(this, "UpdateHealthPercent");
	CurrentHealthText->TextDelegate.BindUFunction(this, "UpdateCurrentHealthText");
	MaxHealthText->TextDelegate.BindUFunction(this, "UpdateMaxHealthText");

	if (TeamResourceList && (ResourceWidgetClass != nullptr))
	{
		UWorld* World = GetWorld();
		ARTFPSGameState* GS = World->GetGameState<ARTFPSGameState>();
		if (GS == nullptr) return false;

		TArray<FResourceUIData> ResourceInfo = GS->GetMapResourceInfo();
		for (int i = 0; i < ResourceInfo.Num(); i++)
		{
			UTeamResourceWidget * child = CreateWidget<UTeamResourceWidget>(World, ResourceWidgetClass);
			child->Setup(ResourceInfo[i]);
			TeamResourceList->AddChild(child);
		}
	}

	return success;
}

UHealthComponent* UFPSUI::GetOwnerHealthComp() const
{
	UHealthComponent* health = nullptr;

	TArray<UHealthComponent*> Healthcomps;
	APawn* MyPawn = GetOwningPlayerPawn();

	if (!MyPawn || MyPawn->IsPendingKill())
	{
		return(health);
	}

	MyPawn->GetComponents<UHealthComponent>(Healthcomps);
	if (Healthcomps.Num() > 0)
	{
		health = Healthcomps[0];
	}
	return health;
}

float UFPSUI::UpdateHealthPercent() const
{
	float percenthealth = 0.0f;
	if (HealthBar == nullptr) return(percenthealth);
	
    
	UHealthComponent* health = GetOwnerHealthComp();
    if (health == nullptr) return(percenthealth);
	

	percenthealth = health->GetHealthPercentage();
	
	return(percenthealth);
}

FText UFPSUI::UpdateCurrentHealthText() const
{
	FText currenttext = FText::FromString("0.0");
	UHealthComponent* health = GetOwnerHealthComp();

	if (health != nullptr)
	{
		float currenthealth = health->GetCurrentHealth();
		
		currenttext = FText::AsNumber(currenthealth, &FloatingPointOptions);
	}

	return currenttext;
}

FText UFPSUI::UpdateMaxHealthText() const
{
	FText currenttext = FText::FromString("0.0");
	UHealthComponent* health = GetOwnerHealthComp();

	if (health != nullptr)
	{
		float maxhealth = health->GetMaxHealth();

		currenttext = FText::AsNumber(maxhealth, &FloatingPointOptions);
	}

	return currenttext;
}
