// Fill out your copyright notice in the Description page of Project Settings.


#include "MageHands.h"
#include "../../CombatCommander.h"

AMageHands::AMageHands() : Super()
{
	bHasSecondary = true;
	if(HasAuthority())
	{
		RightHandAbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("RightHandAbilityComp"));
		check(RightHandAbilityComp);
		RightHandAbilityComp->SetIsReplicated(true);
	}

}


UAbilityComponent * AMageHands::GetHandAbilityComponent(UAbilityAnimNotify * CallingContext) const
{	
	UAbilityComponent * retval = nullptr;

	FName slotname = "";
	if(CallingContext != nullptr)
	{
		slotname = CallingContext->GetAssetSlotName();
	}

	if(slotname == "LeftArm")
	{
		 retval = AbilityComp;
	}
	else if(slotname == "RightArm") 
	{
		retval = RightHandAbilityComp;
	}

	return retval;
}

USkeletalMeshComponent * AMageHands::GetWeaponMesh() const
{
	return(MyPawn->GetPawnMesh());
}

bool AMageHands::InitAbilities()
{
	bool retval = Super::InitAbilities();

	for (int i = 0; i < RightAbilityClasses.Num(); i++)
	{
		RightHandAbilityComp->AddAbility(RightAbilityClasses[i]);
	}

	return(retval);
}

void AMageHands::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);

	#ifndef UE_BUILD_DEBUG
		utilizedcomp->OnReadyNotify();
	#else
		if (utilizedcomp != nullptr)
		{
			utilizedcomp->OnReadyNotify();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AMageHands::OnReadyNotify Failed to Select Arm!")));
		}
	#endif

}


void AMageHands::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	
	#ifndef UE_BUILD_DEBUG
		/*Release Build*/
		utilizedcomp->OnLoopNotify();
	#else
		/*Debug Build*/
		if (utilizedcomp != nullptr)
		{
			utilizedcomp->OnLoopNotify();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AMageHands::OnLoopNotify Failed to Select Arm!")));
		}
	#endif
}

void AMageHands::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	
	#ifndef UE_BUILD_DEBUG
		/*Release Build*/
		utilizedcomp->OnEffectNotify();
	#else
	/*Debug Build*/
	if (utilizedcomp != nullptr)
	{
		utilizedcomp->OnEffectNotify();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AMageHands::OnEffectNotify Failed to Select Arm!")));
	}
	#endif
}

void AMageHands::OnEndNotify(UAbilityAnimNotify * CallingContext) 
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);

	#ifndef UE_BUILD_DEBUG
		/*Release Build*/
		utilizedcomp->OnEndNotify();
	#else
		/*Debug Build*/
		if (utilizedcomp != nullptr)
		{
			utilizedcomp->OnEndNotify();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("AMageHands::OnEndNotify Failed to Select Arm!")));
		}
	#endif
}


void AMageHands::StartSecondaryFire()
{
	ServerStartUseSecondAbility();
}

void AMageHands::StopSecondaryFire()
{
	ServerStopUseSecondAbility();
}

bool AMageHands::ServerStartUseSecondAbility_Validate()
{
	return true;
}

void AMageHands::ServerStartUseSecondAbility_Implementation()
{
	RightHandAbilityComp->StartAbility();
}

bool AMageHands::ServerStopUseSecondAbility_Validate()
{
	return true;
}

void AMageHands::ServerStopUseSecondAbility_Implementation()
{
	RightHandAbilityComp->ReleaseAbility();
}