// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Weapon.generated.h"


/*Forward Declarations*/
class ACombatCommander;

/*Specifies Attatchment Location on Mesh*/
UENUM(BlueprintType)
enum Weapon_Grip_Type
{
	EMPTY_GRIP,
	DAGGER_GRIP,
	ONE_HANDED_GRIP,
	SPEAR_GRIP,
	SHIELD_GRIP,
	TWO_HANDED_GRIP,
	HALBERD_GRIP
};


USTRUCT()
struct FWeaponAnim
{
	GENERATED_USTRUCT_BODY()

	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimFirstPerson;

	/** animation played on pawn (3rd person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimThirdPerson;
};

UCLASS(Abstract, Blueprintable)
class RTS_PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon(const FObjectInitializer& ObjectInitializer);


public:	
	virtual void Equipped(USkeletalMeshComponent * Character, FName Socketname);
	virtual void UnEquipped();
	Weapon_Grip_Type GetType();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	USkeletalMeshComponent * FirstPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	USkeletalMeshComponent * ThirdPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	UBoxComponent * CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	bool bhassecondary = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	FVector SocketLocationOffset = FVector(0,0,0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	FRotator SocketRotationOffset = FRotator(0,0,0);  

public:

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip(const AWeapon* LastWeapon);
	
	/** weapon is now equipped by owner pawn */
	virtual void OnEquipFinished();

	/** weapon is holstered by owner pawn */
	virtual void OnUnEquip();

	/** [server] weapon was added to pawn's inventory */
	virtual void OnEnterInventory(ACombatCommander* NewOwner);

	/** [server] weapon was removed from pawn's inventory */
	virtual void OnLeaveInventory();

protected:
	/** Attaches weapon mesh to pawn's mesh */
	void AttachMeshToPawn();

	/** Detaches weapon mesh from pawn */
	void DetachMeshFromPawn();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	ACombatCommander* MyPawn;


public:

	/** get weapon mesh (needs pawn owner to determine variant) */
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const;

	/** get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	ACombatCommander* GetPawnOwner() const;

protected:
	UFUNCTION()
	void OnRep_MyPawn();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

private:
	Weapon_Grip_Type Grip_Type = EMPTY_GRIP;
};
