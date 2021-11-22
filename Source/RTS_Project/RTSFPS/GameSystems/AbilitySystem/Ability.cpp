// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "AbilityComponent.h"


void UAbility::Init(UAbilityComponent * InComp)
{
	AbilityComp = InComp;
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
	bool bcanplaymontage = !bRequiresReadyCast | AbilityComp->IsCastReady();

	if (ReleaseAbilityMontages.Num() && bcanplaymontage)
	{
		AbilityComp->SetIsCastSuccessful(true);
		float playtime = AbilityComp->PlayAbilityMontage(ReleaseAbilityMontages[0]);
	}
	else
	{
		AbilityComp->SetIsCastSuccessful(false);
		AbilityComp->StopCurrentAnimation();
		AbilityComp->OnCastEnd();
	}
}

void UAbility::OnAbilityEnd()
{
	AbilityComp->OnCastEnd();
}

void UAbility::OnTick(float DeltaSeconds)
{

}

void UAbility::ProcessTarget(AActor * Target)
{
	
}

void UAbility::NotifyOnReady()
{
	AbilityComp->PlayAbilityMontage(LoopMontage);
}

void UAbility::NotifyOnLoop()
{
	AbilityComp->PlayAbilityMontage(LoopMontage);
}

int UAbility::GetManaCost() const
{
	return ManaCost;
}

TEnumAsByte<ECollisionChannel> UAbility::GetAbilityCollisionChannel()
{
	return AbilityEffectChannel;
}

bool UAbility::ConsumeMana(int amount)
{
	return AbilityComp->ConsumeMana(amount);
}

UWorld * UAbility::GetWorld() const
{
	return AbilityComp->GetWorld();
}
bool UAbility::HasAuthority() const
{
	return AbilityComp->HasAuthority();
}
/****************************************DEBUG ONLY**************************************/

/*
#ifdef UE_BUILD_DEBUG
#include "Animation/AbilityAnimNotify.h"

bool UAbility::InitCheck()
{
	bool retval = true;

	for (int i = 0; i < StartAbilityMontages.Num(); i++)
	{
		retval &= AreAnimNotifiesSynced(StartAbilityMontages[i]);
	}

	for (int i = 0; i < ReleaseAbilityMontages.Num(); i++)
	{
		retval &= AreAnimNotifiesSynced(ReleaseAbilityMontages[i]);
	}

	retval &= AreAnimNotifiesSynced(LoopMontage);

	return retval;
}



bool UAbility::AreAnimNotifiesSynced(FAbilityAnim AbilityAnim)
{
	bool retval = true;
	const UAnimMontage * fp = AbilityAnim.AnimFirstPerson;
	const UAnimMontage * tp = AbilityAnim.AnimThirdPerson;

	//Having no Animations is Valid
	if ((!fp && !tp)) return true;

	//If we have one animation we have to have another
	if ((!fp && tp) || (fp && !tp)) return false;


	const TArray<FAnimNotifyEvent> fpnotifies = fp->Notifies;
	const TArray<FAnimNotifyEvent> tpnotifies = tp->Notifies;

	//Remember the last notify we checked in third person and proceed forward after a new notify is found in first person
	int lastsearchedindex = 0;
	
	int fpnotifycount = 0;
	int tpnotifycount = 0;


	for (int i = 0; i < fpnotifies.Num(); i++)
	{
		UAbilityAnimNotify * fpabilitynotify = Cast<UAbilityAnimNotify>(fpnotifies[i].Notify);
		if (fpabilitynotify != nullptr)
		{
			fpnotifycount++;

			for (int k = lastsearchedindex; k < tpnotifies.Num(); k++)
			{
				UAbilityAnimNotify * tpablitynotify = Cast<UAbilityAnimNotify>(tpnotifies[k].Notify);

				if (tpablitynotify != nullptr)
				{
					tpnotifycount++;

					const float fptriggertime = fpnotifies[i].GetTriggerTime();
					const float tptriggertime = tpnotifies[k].GetTriggerTime();

					bool insync = FMath::IsNearlyEqual(fptriggertime, tptriggertime, .1f);
					if (!insync)
					{
						retval &= false;
					}

				}


				lastsearchedindex++;
			}
		}
	}

	retval &= (fpnotifycount == tpnotifycount);

	return retval;
}
uint8 UAbility::GetNotifyConfigID(UAbilityAnimNotify * InNotify)
{
	uint8 retval = 0x00;
	
	const uint8 readybit = 0x01;
	const uint8 loopbit = 0x02;
	const uint8 effect = 0x04;
	const uint8 endbit = 0x08;




	return uint8();
}
#endif
*/

