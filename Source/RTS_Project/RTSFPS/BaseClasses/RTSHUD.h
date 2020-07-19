// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSMinion.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Texture2D.h"
#include "RTSFPSHUD.h"
#include "RTSHUD.generated.h"

class ARTSStructure; 


UCLASS()
class RTS_PROJECT_API ARTSHUD : public ARTSFPSHUD
{
	GENERATED_BODY()
	
public:
	ARTSHUD();

	
	//overriden engine functions to give more control over unit selection
	template <typename ClassFilter>
	bool GetActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<ClassFilter*>& OutActors, bool bIncludeNonCollidingComponents = true, bool bActorMustBeFullyEnclosed = false);

	virtual void GetActorsInSelectionRectangle(TSubclassOf<class AActor> ClassFilter, const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed);
	
	FVector2D Initial_select;  // intial mouse cursor location on click
	FVector2D End_Select;		//mouse cursor location on release
	
	TArray <ARTSMinion*> Selected_Units;
	TArray <ARTSStructure*> Selected_Structure;

	bool SelctionInProcess = false;
	bool StructureSelected = false;



protected:

	UPROPERTY(EditAnywhere)
	float selection_transparency = 0.15f;

	/** Crosshair asset pointer */
	UTexture2D* CrosshairTex;

protected:
	virtual void RTSSelectAndMoveHandler() override;
	virtual void RTSStructureSelectHandler() override;
	virtual void FPSAimAndShootHandler() override;


private:
	void GetSelectedUnits();
	void GetSelectedStructures();
	void CleanSelectedActors();

};


