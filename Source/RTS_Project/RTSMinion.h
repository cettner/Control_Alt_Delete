// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RTSSelectionComponent.h"
#include "RTSMinion.generated.h"

/*Forward Declarations*/
class ACommander;

UCLASS(Blueprintable)
class ARTSMinion : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSMinion();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = UI)
	UTexture* GetThumbnail();

	UPROPERTY(EditDefaultsOnly,Category = UI)
	UTexture* Thumbnail;
	
	bool bismovespecial = false;

	// the location the unit is supposed to be at as instructed by the player controller.
	FVector TargetLocation;

	virtual bool CanInteract(AActor * Interactable);

	virtual AActor* GetTarget();

	virtual void SetTarget(AActor * NewTarget);

	virtual void ClearTarget();

	void SetSelected();

	void SetDeselected();

	void SetUnselectable();

	void SetSelectable();

	virtual void ReleaseAssets();

	virtual bool HasAssets();

	virtual void RtsMove(FVector Local);
	
	virtual void RtsMoveToActor(AActor * move_to_me);

	virtual ACommander * GetCommander();

	virtual void ClearCommander();

	virtual void SetCommander(ACommander * Commander);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int team_index = -1;

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree * RTSBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
	URTSSelectionComponent * Selection;


private:
	UPROPERTY(Replicated)
	ACommander * Cmdr;

protected:


};

