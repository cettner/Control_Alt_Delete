// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "AbilityComponent.h"

#include "Net/UnrealNetwork.h"

void UAbility::Init(UAbilityComponent * InComp)
{
	AbilityComp = InComp;
}

bool UAbility::IsInitialized() const
{
	return AbilityComp != nullptr;
}

void UAbility::OnAbilityStart()
{
	if (StartAbilityMontages.Num())
	{
		float playtime = AbilityComp->PlayAbilityMontage(StartAbilityMontages[0]);
		if (playtime > 0.0f)
		{
			AbilityComp->OnCastStart();
		}
	}
}

void UAbility::OnEffect()
{

}

void UAbility::OnAbilityReleased()
{
	const bool bcanplaymontage = !bRequiresReadyCast || AbilityComp->IsCastReady();

	if (ReleaseAbilityMontages.Num() && bcanplaymontage)
	{
		AbilityComp->SetIsCastSuccessful(true);
		checkf(ReleaseAbilityMontages[0].AnimFirstPerson || ReleaseAbilityMontages[0].AnimThirdPerson, TEXT("UAbility::OnAbilityReleased Montage Slot is created but null"));
		float playtime = AbilityComp->PlayAbilityMontage(ReleaseAbilityMontages[0]);
	}
	else
	{
		AbilityComp->SetIsCastSuccessful(false);
		AbilityComp->StopCurrentAnimation();
		AbilityComp->OnCastEnd();
	}
	AbilityComp->SetIsCastReady(false);
}

void UAbility::OnAbilityEnd()
{
	AbilityComp->OnCastEnd();
}

void UAbility::OnAbilityInterrupted()
{
	AbilityComp->SetIsCastSuccessful(false);
	AbilityComp->SetIsCastReady(false);
	AbilityComp->StopCurrentAnimation();
	AbilityComp->EndAbility();
}

void UAbility::OnTick(float DeltaSeconds)
{

}

void UAbility::SetIsAbilityEnabled(const bool InEnabledState)
{
	if (HasAuthority() && (bIsAbilityEnabled != InEnabledState))
	{
		bIsAbilityEnabled = InEnabledState;

		if (GetNetMode() == ENetMode::NM_DedicatedServer)
		{
			OnRep_bIsAbilityEnabled();
		}
	}
}

FReplicationResourceMap UAbility::GetAbilityCost() const
{
	return AbilityCost;
}

void UAbility::NotifyOnReady()
{
	AbilityComp->PlayAbilityMontage(LoopMontage);
}

void UAbility::NotifyOnLoop()
{
	AbilityComp->PlayAbilityMontage(LoopMontage);
}

TEnumAsByte<ECollisionChannel> UAbility::GetAbilityCollisionChannel() const
{
	return AbilityEffectChannel;
}

TArray<FName> UAbility::GetAbilityTags() const
{
	return AbilityTags;
}

UWorld * UAbility::GetWorld() const
{
	return AbilityComp->GetWorld();
}

bool UAbility::HasAuthority() const
{
	return AbilityComp->HasAuthority();
}

ENetMode UAbility::GetNetMode() const
{
	return AbilityComp->GetNetMode();
}

void UAbility::OnRep_bIsAbilityEnabled()
{
	if (AbilityComp != nullptr)
	{
		AbilityComp->OnAbilityEnableStateChanged(this);
	}
}

void UAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
	DOREPLIFETIME(UAbility, bIsAbilityEnabled);
}

bool UAbility::IsSupportedForNetworking() const
{
	return true;
}

