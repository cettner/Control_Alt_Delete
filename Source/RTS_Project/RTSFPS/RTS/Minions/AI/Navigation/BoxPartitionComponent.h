// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BoxPartitionComponent.generated.h"

#define INVALID_PARTITION_ID -1



USTRUCT()
struct FPartitionNeighbor
{
	GENERATED_USTRUCT_BODY()

public:
	FPartitionNeighbor()
	{

	}

	FPartitionNeighbor(class UBoxPartitionComponent* InNeighborTile, FVector InIsCornerNeighbor)
	{
		NeighborPartition = InNeighborTile;
		NeighborDirection = InIsCornerNeighbor;
	}

	friend bool operator == (const FPartitionNeighbor& Myself, const FPartitionNeighbor& Other)
	{
		const bool retval = (Myself.NeighborPartition == Other.NeighborPartition) && (Myself.NeighborDirection == Other.NeighborDirection);
		return retval;
	}

	FVector NeighborDirection = FVector();
	UBoxPartitionComponent* NeighborPartition = nullptr;
};

UCLASS()
class RTS_PROJECT_API UBoxPartitionComponent : public UBoxComponent
{
	GENERATED_BODY()


	friend class UBoidBoundsComponent;

	UBoxPartitionComponent();

public:
	FORCEINLINE int32 Num() const { return PartitionActors.Num(); }
	FORCEINLINE const TSet<UBoxPartitionComponent*>& GetNeighbors() const { return PartitionNeighbors; }
	FORCEINLINE const TSet<AActor*>& GetActors() const { return PartitionActors; }
	FORCEINLINE uint32  GetPartitionID() const { return PartitionID; }

protected:

	FORCEINLINE void SetPartitionID(int32 InID) { PartitionID = InID; }

	UFUNCTION()
	virtual void OnActorOverlapped (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual bool ShouldAddActor(const AActor* InActor) const;

	virtual void AddActor(AActor* InActor);
	virtual bool RemoveActor(const AActor* InActor);

	virtual void InitBounds();
	virtual void AddNeighbor(UBoxPartitionComponent* InPartition);

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


#if WITH_EDITOR
public:
	virtual void EnableDebug(bool bInEnable, bool InShouldShowNeighbors = false);

protected:
	virtual void DrawDebug();
#endif


protected:
	int32 PartitionID = INVALID_PARTITION_ID;

	TSet<UBoxPartitionComponent*> PartitionNeighbors = TSet<UBoxPartitionComponent*>();
	TSet<AActor*> PartitionActors = TSet<AActor*>();

#if WITH_EDITORONLY_DATA
protected:
	UPROPERTY(EditDefaultsOnly)
	FColor BaseDebugColor = FColor::Green;

	UPROPERTY(EditDefaultsOnly)
	FColor BaseNeighborDebugColor = FColor::Blue;

protected:
	bool bIsDrawingBaseBox = false;
	bool bIsDrawingNeighbors = false;
#endif
};
