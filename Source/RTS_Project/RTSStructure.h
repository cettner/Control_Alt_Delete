// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "RTSHUD.h"
#include "RTSPlayerController.h"
#include "RTSStructure.generated.h"

UCLASS()
class RTS_PROJECT_API ARTSStructure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTSStructure(const FObjectInitializer& ObjectInitializer);

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(Category = Default)
	void OnClick(AActor* Target, FKey ButtonPressed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = StaticMeshComponents)
	UStaticMeshComponent* Mesh;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	void SetSelected();
	void SetDeselected();

private:
	ARTSHUD* HudPtr;
	ARTSPlayerController* PC;
};
