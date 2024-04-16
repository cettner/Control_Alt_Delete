// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxPartitionComponent.h"
#include "BoidBoundsComponent.h"

UBoxPartitionComponent::UBoxPartitionComponent() : Super()
{
	if (!IsTemplate())
	{
		PrimaryComponentTick.bCanEverTick = false;
		OnComponentBeginOverlap.AddDynamic(this, &UBoxPartitionComponent::OnActorOverlapped);
		OnComponentEndOverlap.AddDynamic(this, &UBoxPartitionComponent::OnOverlapEnd);
	}
}

void UBoxPartitionComponent::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ShouldAddActor(OtherActor))
	{
		AddActor(OtherActor);
		UBoidBoundsComponent * owningbounds = Cast<UBoidBoundsComponent>(GetOuter());
		UBoxPartitionComponent * outpartition = owningbounds->GetPartitionFromPosition(OtherActor->GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Entering Partition %d Count %d"), GetPartitionID(), GetNeighbors().Num()));
	}
}

void UBoxPartitionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PartitionActors.Contains(OtherActor))
	{
		RemoveActor(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Leaving Partition %d Count %d"), GetPartitionID(), GetNeighbors().Num()));
	}
}

bool UBoxPartitionComponent::ShouldAddActor(const AActor* InActor) const
{
	const bool retval = Cast<APawn>(InActor) != nullptr;
	return retval;
}

void UBoxPartitionComponent::AddActor(AActor* InActor)
{
	PartitionActors.Add(InActor);
}

bool UBoxPartitionComponent::RemoveActor(const AActor* InActor)
{
	const int32& removecount = PartitionActors.Remove(InActor);
	return removecount > 0;
}

void UBoxPartitionComponent::InitBounds()
{
	TSet<AActor*> overlappedactors;
	GetOverlappingActors(overlappedactors, APawn::StaticClass());

	for (AActor* overlappedactor : overlappedactors)
	{
		AddActor(overlappedactor);
	}
}

void UBoxPartitionComponent::AddNeighbor(UBoxPartitionComponent* InPartition)
{
	PartitionNeighbors.Emplace(InPartition);
}

void UBoxPartitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PartitionActors.Num())
	{
		EnableDebug(true, true);
	}
	else if(PartitionActors.Num() == 0)
	{
		EnableDebug(false);
	}

}

#if WITH_EDITORONLY_DATA
void UBoxPartitionComponent::EnableDebug(bool bInEnable, bool InShouldShowNeighbors)
{
	bIsDrawingBaseBox = bInEnable;
	bIsDrawingNeighbors = InShouldShowNeighbors;
	DrawDebug();
}

void UBoxPartitionComponent::DrawDebug()
{
	if (bIsDrawingBaseBox)
	{
		DrawDebugBox(GetWorld(), GetComponentLocation(), GetScaledBoxExtent(), BaseDebugColor, false, 2.0, 0, 6);
	}
	if (bIsDrawingNeighbors)
	{
		for (UBoxPartitionComponent* neighbor : GetNeighbors())
		{
			DrawDebugBox(GetWorld(), neighbor->GetComponentLocation(), neighbor->GetScaledBoxExtent(), BaseNeighborDebugColor, false, 2.0, 0, 5);
		}
	}
}
#endif