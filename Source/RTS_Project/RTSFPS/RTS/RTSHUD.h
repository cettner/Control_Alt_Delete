// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"

#include "RTSMinion.h"
#include "../GameArchitecture/RTSFPSHUD.h"
#include "RTSHUD.generated.h"

class ARTSStructure; 


UCLASS()
class RTS_PROJECT_API ARTSHUD : public ARTSFPSHUD
{
	GENERATED_BODY()
	
public:
	virtual FVector2D GetMouseLocation() const;


protected:
	//overriden engine functions to give more control over unit selection
	template <typename ClassFilter>
	bool GetRTSActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<IRTSObjectInterface*>& OutActors, bool bIncludeNonCollidingComponents = true, bool bActorMustBeFullyEnclosed = false);

	virtual void GetRTSActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed, TSubclassOf<AActor> ActorClass = AActor::StaticClass());

protected:
	/*AHUD overrides*/
	virtual void DrawHUD() override;
	
protected:
	/*DefaultHUD Overrides*/
	virtual bool ClientInitializeHUD() override;

protected:
	virtual void RTSSelectAndMoveHandler();

private:
	void CleanSelectedActors();
	bool IsBoxSelectionEnabled() const;
	void SetBoxSelectionEnabled(const bool InEnabled);
	void ScanSelectedUnits();


public:
	void StartRTSSelection(const FVector2D InStartScreenPosition, const TArray<IRTSObjectInterface*> &StartingSelection = TArray<IRTSObjectInterface*>());
	bool IsSelectionInProgress() const;
	TArray <IRTSObjectInterface*> FinishRTSSelection();
	bool ForceRemoveSelection(IRTSObjectInterface* InObject);

protected:
	FVector2D SelectionStartPosition;  // intial mouse cursor location on click

	TArray <IRTSObjectInterface *> Selected_Units = TArray<IRTSObjectInterface*>();
	TArray <IRTSObjectInterface*> RetainedSelection = TArray<IRTSObjectInterface*>();

	bool bIsSelectionInProcess = false;


protected:
	UPROPERTY(EditDefaultsOnly)
	float SelectionAlpha = 0.15f;

	/** Crosshair asset pointer */
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTex = nullptr;

protected:
	bool bIsBoxSelectEnabled = false;

};


