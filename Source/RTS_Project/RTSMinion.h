// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "RTSMinion.generated.h"

UCLASS(Blueprintable)
class ARTSMinion : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSMinion();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }


	bool bismovespecial = false;

	void SetSelected();

	void SetDeselected();

	virtual void ReleaseAssets(FVector Order_Local);

	virtual void ReleaseAssets();

	virtual bool HasAssets();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float health;
	float damage;



private:

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
	


};

