// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../BaseClasses/RTSMinion.h"
#include "SiegeProjectile.h"
#include "RTSCatapult.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSCatapult : public ARTSMinion
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void RtsMove(FVector Local) override;
	virtual void RtsMoveToActor(AActor * move_to_me) override;

public:
	UPROPERTY(EditDefaultsOnly,Category = Debug)
	float LaunchTime = 5.0;
	void LaunchProjectile();
	void Reload();
	ARTSCatapult();
	
	UPROPERTY(EditAnywhere, Category = Gameplay)
	float Min_Range = 500.0;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	class USphereComponent* Min_Range_Sphere;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float Max_Range = 1000.0;

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	class USphereComponent* Max_Range_Sphere;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ASiegeProjectile> Projectile;

private:
	FTimerHandle Launch_Handler;
	UPROPERTY(EditDefaultsOnly, Category = Debug)
	FVector LaunchLocal;

	UPROPERTY(EditDefaultsOnly, Category = Debug)
	FRotator LaunchRot;

	TArray<AActor*> In_Range;
	bool Target_Available;

	bool ShouldTurn(AActor* TurnTo);
	bool fireready = true;

	UFUNCTION(Category = Gameplay)
	void OnMinRangeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(Category = Gameplay)
	void OnMinRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Category = Gameplay)
	void OnMaxRangeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(Category = Gameplay)
	void OnMaxRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
