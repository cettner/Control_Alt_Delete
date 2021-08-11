// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimMontage.h"
#include "Ability.generated.h"


USTRUCT()
struct FAbilityAnim
{
	GENERATED_USTRUCT_BODY()

	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimFirstPerson;

	/** animation played on pawn (3rd person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimThirdPerson;
};

/*Forward Declaration*/
class UAbilityComponent;

UCLASS(Blueprintable)
class RTS_PROJECT_API UAbility : public UObject
{
	GENERATED_BODY()

	public:
		virtual void Init(UAbilityComponent * InComp);

		virtual void OnAbilityStart();
		virtual void OnEffect();
		virtual void OnAbilityReleased();
		virtual void OnAbilityEnd();

	public:
		/*Called from AnimNotify to Set Ability is Ready for release*/
		virtual void NotifyOnReady();
		virtual void NotifyOnLoop();

	public:
		int GetManaCost() const;
		TEnumAsByte<ECollisionChannel> GetAbilityCollisionChannel();
		virtual bool ConsumeMana(int amount);

	protected:
		virtual void InitializeAnimNotify(UAnimNotify * InNotify);

	protected:
		UPROPERTY(EditDefaultsOnly)
		/*Cost of the Abiliy*/
		int ManaCost = 0;

		UPROPERTY(EditDefaultsOnly)
		/*If true, Release Events or Effects will not fire unless bIsCastReady is set to true, otherwise they will always fire*/
		bool bRequiresReadyCast = true;

		UPROPERTY(EditDefaultsOnly)
		TArray<FAbilityAnim> StartAbilityMontages;

		UPROPERTY(EditDefaultsOnly)
		FAbilityAnim LoopMontage;

		UPROPERTY(EditDefaultsOnly)
		TArray<FAbilityAnim> ReleaseAbilityMontages;

		UPROPERTY(EditDefaultsOnly)
		FName EffectSocketName;

		UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<ECollisionChannel>  AbilityEffectChannel;


	protected:
		UAbilityComponent * AbilityComp = nullptr;

		bool bIsCastReady = false;

	protected:
		FTransform Transform = FTransform();

};
