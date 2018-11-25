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
#include "RTSHUD.generated.h"

class ARTSStructure; 
/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ARTSHUD();
	virtual void DrawHUD() override;  // HUD "tick" function
	
	//overriden engine functions to give more control over unit selection
	template <typename ClassFilter>
	bool GetActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<ClassFilter*>& OutActors, bool bIncludeNonCollidingComponents = true, bool bActorMustBeFullyEnclosed = false)
	{
		//Is Actor subclass?
		if (!ClassFilter::StaticClass()->IsChildOf(AActor::StaticClass()))
		{
			return false;
		}

		//Run Inner Function, output to Base AActor Array
		TArray<AActor*> OutActorsBaseArray;
		GetActorsInSelectionRectangle(ClassFilter::StaticClass(), FirstPoint, SecondPoint, OutActorsBaseArray, bIncludeNonCollidingComponents, bActorMustBeFullyEnclosed);

		//Construct casted template type array
		for (AActor* EachActor : OutActorsBaseArray)
		{
			OutActors.Add(CastChecked<ClassFilter>(EachActor));
		}

		return true;
	};
	virtual void GetActorsInSelectionRectangle(TSubclassOf<class AActor> ClassFilter, const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, bool bIncludeNonCollidingComponents, bool bActorMustBeFullyEnclosed);
	
	
	FVector2D Initial_select;  // intial mouse cursor location
	FVector2D End_Select;		//mouse cursor location on release
	
	FVector2D GetMouseLocation();

	TArray <ARTSMinion*> Selected_Units;
	TArray <ARTSStructure*> Selected_Structure;

	bool SelctionInProcess = false;
	bool StructureSelected = false;

	enum HUDSTATE {
		LBOUND,
		RTS_SELECT_AND_MOVE,
		RTS_STRUCTURE_SELECT,
		UBOUND
	};

	HUDSTATE state;


	UFUNCTION(BlueprintCallable, Category = HUD)
		void Change_HUD_State(int statetype);

	UPROPERTY(EditAnywhere)
		float selection_transparency = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool bIsRTSCharacter = true;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
	void GetSelectedUnits();
	void GetSelectedStructures();
	void CleanSelectedActors();
	void RTSSelectAndMoveHandler();
	void RTSStructureSelectHandler();



};
