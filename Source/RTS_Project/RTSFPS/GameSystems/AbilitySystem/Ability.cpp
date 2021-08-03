// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "AbilityComponent.h"
#include "Animation/AbilityAnimNotify.h"

void UAbility::Init(UAbilityComponent * InComp)
{

	AbilityComp = InComp;

	/*Get all the anim Notifies from all our Animations and Initialize them*/
	for (int i = 0; i < StartAbilityMontages.Num(); i++)
	{
		TArray<FAnimNotifyEvent> NotifiesFP;
		TArray<FAnimNotifyEvent> NotifiesTP;

		/*First Person*/
		if (IsValid(StartAbilityMontages[i].AnimFirstPerson))
		{
			NotifiesFP = StartAbilityMontages[i].AnimFirstPerson->Notifies;
		}
		for (int k = 0; k < NotifiesFP.Num(); k++)
		{
			InitializeAnimNotify(NotifiesFP[k].Notify);
		}


		/*Thrid Person*/
		if (IsValid(StartAbilityMontages[i].AnimThirdPerson))
		{
			NotifiesTP = StartAbilityMontages[i].AnimThirdPerson->Notifies;
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
	if (ReleaseAbilityMontages.Num())
	{
		float playtime = AbilityComp->PlayAbilityMontage(ReleaseAbilityMontages[0]);
		if (playtime > 0.0f)
		{
		}
	}
}

void UAbility::OnAbilityEnd()
{
}

void UAbility::NotifyOnReady()
{
}

int UAbility::GetManaCost() const
{
	return ManaCost;
}

bool UAbility::ConsumeMana(int amount)
{
	return AbilityComp->ConsumeMana(amount);
}


