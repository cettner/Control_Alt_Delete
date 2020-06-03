// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "RTSSelectionComponent.generated.h"

#define SELECTION_CHANNEL  ECC_GameTraceChannel1

enum SELECTION_TYPE
{
	CLICK_AND_CLEAR
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API URTSSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSSelectionComponent();

	void SetSelected();
	void SetDeselected();
	void EnableSecondary();
	void DisableSecondary();
	void SetRoot(USceneComponent* RootComponent);
	void SetDetection(UPrimitiveComponent * Collision);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	/*Used for normal togglable selection*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
	class UDecalComponent* PrimarySelectionRing;

	/*Used to designate permanent designatable objects, like enemies, or teammates*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
	class UDecalComponent* SecondarySelectionRing;
};
