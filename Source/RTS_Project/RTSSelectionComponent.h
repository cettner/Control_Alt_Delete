// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "RTSSelectionComponent.generated.h"

#define SELECTION_CHANNEL  ECC_GameTraceChannel1

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API URTSSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSSelectionComponent();
	
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	void SetSelected();
	void SetDeselected();
	void SetRoot(USceneComponent* RootComponent);
	void SetDetection(UPrimitiveComponent * Collision);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool bcanclearselection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UDecalComponent* CursorToWorld;
};
