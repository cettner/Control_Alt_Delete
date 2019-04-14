// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum Socket_Types
{
	LEFTHAND,
	RIGHTHAND
};

UCLASS()
class RTS_PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool Equipped();
	virtual bool UnEquipped();

	static const FString Equipped_Socket_Name;

	static const FString UnEquipped_Socket_Name;

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent * Mesh;
};
