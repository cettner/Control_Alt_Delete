// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "AbilityComponent.h"
#include "Animation/AbilityAnimNotify.h"

void UAbility::Init(UAbilityComponent * InComp)
{

	AbilityComp = InComp;

	/*Get all the anim Notifies from all our Animations and Initialize them*/

	/*Start Montages*/
	for (int i = 0; i < StartAbilityMontages.Num(); i++)
	{
		TArray<FAnimNotifyEvent> NotifiesFP;
		TArray<FAnimNotifyEvent> NotifiesTP;

		/*First Person Start*/
		if (IsValid(StartAbilityMontages[i].AnimFirstPerson))
		{
			NotifiesFP = StartAbilityMontages[i].AnimFirstPerson->Notifies;
		}
		for (int k = 0; k < NotifiesFP.Num(); k++)
		{
			InitializeAnimNotify(NotifiesFP[k].Notify);
		}


		/*Third Person Start*/
		if (IsValid(StartAbilityMontages[i].AnimThirdPerson))
		{
			NotifiesTP = StartAbilityMontages[i].AnimThirdPerson->Notifies;
		}

		for (int k = 0; k < NotifiesTP.Num(); k++)
		{
			InitializeAnimNotify(NotifiesFP[k].Notify);
		}
	}

	/*Loop Montages*/
	/*First Person*/
	if (IsValid(LoopMontage.AnimFirstPerson))
	{
		TArray<FAnimNotifyEvent> NotifiesFP = LoopMontage.AnimFirstPerson->Notifies;
		
		for (int k = 0; k < NotifiesFP.Num(); k++)
		{
			InitializeAnimNotify(NotifiesFP[k].Notify);
		}
	}
	/*Third Person*/
	if (IsValid(LoopMontage.AnimThirdPerson))
	{
		TArray<FAnimNotifyEvent> NotifiesTP = LoopMontage.AnimThirdPerson->Notifies;
		for (int k = 0; k < NotifiesTP.Num(); k++)
		{
			InitializeAnimNotify(NotifiesTP[k].Notify);
		}
	}


	/*Release Montages*/
	for (int i = 0; i < ReleaseAbilityMontages.Num(); i++)
	{
		TArray<FAnimNotifyEvent> NotifiesFP;
		TArray<FAnimNotifyEvent> NotifiesTP;

		/*First Person Release*/
		if (IsValid(ReleaseAbilityMontages[i].AnimFirstPerson))
		{
			NotifiesFP = ReleaseAbilityMontages[i].AnimFirstPerson->Notifies;
		}
		for (int k = 0; k < NotifiesFP.Num(); k++)
		{
			InitializeAnimNotify(NotifiesFP[k].Notify);
		}


		/*Third Person Release*/
		if (IsValid(ReleaseAbilityMontages[i].AnimThirdPerson))
		{
			NotifiesTP =  ReleaseAbilityMontages[i].AnimThirdPerson->Notifies;
		}

		for (int k = 0; k < NotifiesTP.Num(); k++)
		{
			InitializeAnimNotify(NotifiesFP[k].Notify);
		}
	}
}

void UAbility::InitializeAnimNotify(UAnimNotify * InNotify)
{
	UAbilityAnimNotify * AbilityNotify = Cast<UAbilityAnimNotify>(InNotify);
	if (IsValid(InNotify))
	{
		AbilityNotify->InitializeAbilities(AbilityComp);
	}

}

void UAbility::OnAbilityStart()
{
	if (StartAbilityMontages.Num())
	{
		float playtime = AbilityComp->PlayAbilityMontage(StartAbilityMontages[0]);
		if (playtime > 0.0f)
		{
			AbilityComp->OnCastNotify();
		}
	}
}

void UAbility::OnEffect()
{

}

void UAbility::OnAbilityReleased()
{
	bool bcanplaymontage = !bRequiresReadyCast | bIsCastReady;

	if (ReleaseAbilityMontages.Num() && bcanplaymontage)
	{
		float playtime = AbilityComp->PlayAbilityMontage(ReleaseAbilityMontages[0]);
	}
	bIsCastReady = false;
}

void UAbility::OnAbilityEnd()
{

}

void UAbility::NotifyOnReady()
{
	bIsCastReady = true;
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


