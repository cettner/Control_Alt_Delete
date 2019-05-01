// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Weapon.generated.h"

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

UCLASS()
class RTS_PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Equipped(USkeletalMeshComponent * Character, FName Socketname);
	virtual void UnEquipped();
	Weapon_Grip_Type GetType();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	USkeletalMeshComponent * Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	UBoxComponent * CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	bool bhassecondary = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	FVector SocketLocationOffset = FVector(0,0,0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	FRotator SocketRotationOffset = FRotator(0,0,0);  

private:
	Weapon_Grip_Type Grip_Type = EMPTY_GRIP;
};
