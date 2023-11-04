// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimMontage.h"

#include "../ResourceSystem/Resource.h"
#include "Ability.generated.h"


USTRUCT()
struct FAbilityAnim
{
	GENERATED_USTRUCT_BODY()

	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimFirstPerson = nullptr;

	/** animation played on pawn (3rd person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimThirdPerson = nullptr;

	bool operator==(const FAbilityAnim& Other) const { return (AnimFirstPerson == Other.AnimFirstPerson) && (AnimThirdPerson == Other.AnimThirdPerson); }

};

/*Forward Declaration*/
class UAbilityComponent;

UCLASS(Blueprintable)
class RTS_PROJECT_API UAbility : public UObject
{
	GENERATED_BODY()

	public:
		virtual void Init(UAbilityComponent * InComp);
		virtual bool IsInitialized() const;

		virtual void OnAbilityStart();
		virtual void OnEffect();
		virtual void OnAbilityReleased();
		virtual void OnAbilityEnd();
		virtual void OnAbilityInterrupted();
		virtual void OnTick(float DeltaSeconds);

	public:
		virtual void SetIsAbilityEnabled(const bool InEnabledState);
		FORCEINLINE bool IsAbilityEnabled() const { return bIsAbilityEnabled; };
		virtual FReplicationResourceMap GetAbilityCost() const ;

	public:
		/*Called from AnimNotify to Set Ability is Ready for release*/
		virtual void NotifyOnReady();
		virtual void NotifyOnLoop();

	public:
		TEnumAsByte<ECollisionChannel> GetAbilityCollisionChannel() const;
		bool GetDefaultEnabledState() const;
		TArray<FName> GetAbilityTags() const;

	protected:
		UWorld * GetWorld() const;
		bool HasAuthority() const;
		ENetMode GetNetMode() const;

	protected:
		UFUNCTION()
		virtual void OnRep_bIsAbilityEnabled();

	protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
		virtual bool IsSupportedForNetworking() const override;

	protected:
		UPROPERTY(EditDefaultsOnly)
		/*If true, Release Events or Effects will not fire unless bIsCastReady from interface is set to true, otherwise they will always fire*/
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

		UPROPERTY(EditDefaultsOnly)
		bool DefaultEnabledState = true;

		UPROPERTY(EditDefaultsOnly)
		TArray<FName> AbilityTags = TArray<FName>();

	protected:
		UAbilityComponent * AbilityComp = nullptr;

		UPROPERTY(ReplicatedUsing = OnRep_bIsAbilityEnabled)
		bool bIsAbilityEnabled = DefaultEnabledState;

		UPROPERTY(EditDefaultsOnly)
		FReplicationResourceMap AbilityCost = FReplicationResourceMap();
};
