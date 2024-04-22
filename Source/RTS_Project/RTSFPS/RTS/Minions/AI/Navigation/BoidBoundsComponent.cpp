// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidBoundsComponent.h"

UBoidBoundsComponent::UBoidBoundsComponent() : Super()
{
}

UBoxPartitionComponent* UBoidBoundsComponent::GetPartitionFromPosition(const FVector& InPosition) const
{
	UBoxPartitionComponent * retval = nullptr;
	 
	if (IsValidBoundsPoint(InPosition))
	{
		const FVector& startcorner = GetMinBounds();
		const FVector& positiondiff = InPosition - startcorner;

		const int32 approxcol  = floorf(positiondiff.X * FractionalPartitionLength);
		const int32 approxrow  = floorf(positiondiff.Y * FractionalPartitionWidth);
		const int32 approxaile = floorf(positiondiff.Z * FractionalPartitionHeight);

		const int32 approxindex = approxcol + (approxrow * LengthPartitions) + (approxaile * LengthPartitions * WidthPartitions);

		if (approxindex > INVALID_PARTITION_ID && approxindex < BoundsPartitions.Num())
		{
			retval = BoundsPartitions[approxindex];
		}
	}

	return retval;
}

bool UBoidBoundsComponent::IsValidBoundsPoint(const FVector& InPosition) const
{
	bool retval = true;
	const FVector& mincorner = GetMinBounds();
	const FVector& maxcorner = GetMaxBounds();
	if (InPosition.X > maxcorner.X
		|| InPosition.Y > maxcorner.Y
		|| InPosition.Z > maxcorner.Z
		|| InPosition.X < mincorner.X
		|| InPosition.Y < mincorner.Y
		|| InPosition.Z < mincorner.Z)
	{
		retval = false;
	}

	return retval;
}

FPrimitiveSceneProxy* UBoidBoundsComponent::CreateSceneProxy()
{
	class FMultiBoxSceneProxy final : public FPrimitiveSceneProxy
	{
	public:
		SIZE_T GetTypeHash() const override
		{
			static size_t UniquePointer;
			return reinterpret_cast<size_t>(&UniquePointer);
		}

		FMultiBoxSceneProxy(const UBoidBoundsComponent* InComponent)
			: FPrimitiveSceneProxy(InComponent)
			, bDrawOnlyIfSelected(InComponent->bDrawOnlyIfSelected)
			, BoxExtents(InComponent->GetScaledBoxExtent())
			, BoxColor(InComponent->ShapeColor)
			, BoxScale(InComponent->GetComponentScale())
			, LineThickness(InComponent->LineThickness)
			, LengthPartitionCount(InComponent->LengthPartitions)
			, HeightPartitionCount(InComponent->HeightPartitions)
			, WidthPartitionCount(InComponent->WidthPartitions)
		{
			bWillEverBeLit = false;
		}

		virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
		{
			QUICK_SCOPE_CYCLE_COUNTER(STAT_BoxSceneProxy_GetDynamicMeshElements);

			const FMatrix& LocalToWorld = GetLocalToWorld();

			for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
			{
				if (VisibilityMap & (1 << ViewIndex))
				{
					const FSceneView* View = Views[ViewIndex];

					const FLinearColor DrawColor = GetViewSelectionColor(BoxColor, *View, IsSelected(), IsHovered(), false, IsIndividuallySelected());

					FVector outextent;
					TArray<FVector> outcenters;
					CalculateBoxCenters(LocalToWorld.GetOrigin(), BoxExtents, LengthPartitionCount, WidthPartitionCount, HeightPartitionCount, outcenters, outextent);
					FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
					
	 				for (int32 i = 0; i < outcenters.Num(); i++)
					{
						DrawOrientedWireBox(PDI, outcenters[i], LocalToWorld.GetUnitAxis(EAxis::X), LocalToWorld.GetUnitAxis(EAxis::Y), LocalToWorld.GetUnitAxis(EAxis::Z), outextent, DrawColor, SDPG_World, LineThickness);
					}
				}
			}
		}

		virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
		{
			const bool bProxyVisible = !bDrawOnlyIfSelected || IsSelected();

			// Should we draw this because collision drawing is enabled, and we have collision
			const bool bShowForCollision = View->Family->EngineShowFlags.Collision && IsCollisionEnabled();

			FPrimitiveViewRelevance Result;
			Result.bDrawRelevance = (IsShown(View) && bProxyVisible) || bShowForCollision;
			Result.bDynamicRelevance = true;
			Result.bShadowRelevance = IsShadowCast(View);
			Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
			return Result;
		}
		virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }
		uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

	private:
		const uint32	bDrawOnlyIfSelected : 1;
		const FVector   BoxScale;
		const FVector	BoxExtents;
		const FColor	BoxColor;
		const float LineThickness;
		uint32 LengthPartitionCount = 1U;
		uint32 HeightPartitionCount = 1U;
		uint32 WidthPartitionCount  = 1U;
	};

    return new FMultiBoxSceneProxy(this);
}

void UBoidBoundsComponent::CalculateBoxCenters(const FVector& InBoxCenter, const FVector& InBoxExtent, const uint32 InNumLengthPartitions, const uint32 InNumWidthPartitions, const uint32 InNumHeightPartitions, TArray<FVector>& OutCenters, FVector& OutExtent)
{
	// Calculate the size of each partition along each dimension
	const FVector PartitionSize(
		InBoxExtent.X * 2 / InNumLengthPartitions,
		InBoxExtent.Y * 2 / InNumWidthPartitions,
		InBoxExtent.Z * 2 / InNumHeightPartitions
	);

	// Calculate the starting corner of the partitioned space
	const FVector StartCorner = InBoxCenter - InBoxExtent;
	// Clear the output array
	OutCenters.Empty();

	// Loop through each partition along each dimension and calculate the center of each box
	for (uint32 Z = 0; Z < InNumHeightPartitions; ++Z)
	{
		for (uint32 Y = 0; Y < InNumWidthPartitions; ++Y)
		{
			for (uint32 X = 0; X < InNumLengthPartitions; ++X)
			{
				// Calculate the center of the current box
				const FVector BoxCenter = StartCorner + FVector(
					PartitionSize.X * (0.5f + X),
					PartitionSize.Y * (0.5f + Y),
					PartitionSize.Z * (0.5f + Z)
				);
				// Add the center to the output array
				OutCenters.Add(BoxCenter);
			}
		}
	}

	// Calculate the extent of all the new boxes
	OutExtent = (InBoxExtent) / FVector(InNumLengthPartitions, InNumWidthPartitions, InNumHeightPartitions);
}

void UBoidBoundsComponent::InitializeBoundsNeighbors()
{
	for (int32 i = 0; i < BoundsPartitions.Num(); i++)
	{
		UBoxPartitionComponent * currentpartition = BoundsPartitions[i];
		const uint32 col = (i % LengthPartitions) % (LengthPartitions * WidthPartitions);
		const uint32 row = (i / LengthPartitions) % (LengthPartitions * WidthPartitions);
		const uint32 aisle = i / (LengthPartitions * WidthPartitions);

		// Positive Y
		if (row < (WidthPartitions - 1))
		{
			currentpartition->AddNeighbor(BoundsPartitions[i + LengthPartitions]);

			//Positive X Positive Y
			if (col < LengthPartitions - 1)
			{
				currentpartition->AddNeighbor(BoundsPartitions[i + LengthPartitions + 1]);
			}
			//Negative X Positive Y
			if (col > 0)
			{
				currentpartition->AddNeighbor(BoundsPartitions[i + LengthPartitions - 1]);
			}
		}
		// Negative Y
		if (row > 0)
		{
			currentpartition->AddNeighbor(BoundsPartitions[i - LengthPartitions]);

			//Positive X Negative Y
			if (col < LengthPartitions - 1)
			{
				currentpartition->AddNeighbor(BoundsPartitions[i - LengthPartitions + 1]);
			}
			//Negative X Negative Y
			if (col > 0)
			{
				currentpartition->AddNeighbor(BoundsPartitions[i - LengthPartitions - 1]);
			}
		}
		// Negative X
		if (col > 0)
		{
			currentpartition->AddNeighbor(BoundsPartitions[i - 1]);
		}

		// Positive X
		if (col < LengthPartitions - 1)
		{
			currentpartition->AddNeighbor(BoundsPartitions[i + 1]);
		}

		// Positive Z
		if (aisle < HeightPartitions - 1)
		{
			const int32 aboveindex = i + (LengthPartitions * WidthPartitions);
			currentpartition->AddNeighbor(BoundsPartitions[aboveindex]);

			// Positive X Positive Z
			if (col < (LengthPartitions - 1))
			{
				currentpartition->AddNeighbor(BoundsPartitions[aboveindex + 1]);
			}
			// Negative X Positive Z
			if (col > 0)
			{
				currentpartition->AddNeighbor(BoundsPartitions[aboveindex - 1]);
			}
			// Positive Y Positive Z
			if (row < (WidthPartitions - 1))
			{
				currentpartition->AddNeighbor(BoundsPartitions[aboveindex + LengthPartitions]);

				//Positive X Positive Y Positive Z
				if (col < LengthPartitions - 1)
				{
					currentpartition->AddNeighbor(BoundsPartitions[aboveindex + LengthPartitions + 1]);
				}
				//Negative X Positive Y Positive Z
				if (col > 0)
				{
					currentpartition->AddNeighbor(BoundsPartitions[aboveindex + LengthPartitions - 1]);
				}

			}
			// Negative Y Positive Z
			if (row > 0)
			{
				currentpartition->AddNeighbor(BoundsPartitions[aboveindex - LengthPartitions]);

				//Positive X Negative Y Positive Z
				if (col < LengthPartitions - 1)
				{
					currentpartition->AddNeighbor(BoundsPartitions[aboveindex - LengthPartitions + 1]);
				}
				//Negative X Negative Y Positive Z
				if (col > 0)
				{
					currentpartition->AddNeighbor(BoundsPartitions[aboveindex - LengthPartitions - 1]);
				}
			}

		}

		// Negative Z
		if (aisle > 0)
		{
			const int32 belowindex = i - (LengthPartitions * WidthPartitions);
			currentpartition->AddNeighbor(BoundsPartitions[belowindex]);

			// Positive X Negative Z
			if (col < (LengthPartitions - 1))
			{
				currentpartition->AddNeighbor(BoundsPartitions[belowindex + 1]);
			}
			// Negative X Negative Z
			if (col > 0)
			{
				currentpartition->AddNeighbor(BoundsPartitions[belowindex - 1]);
			}

			// Positive Y Negative Z
			if (row < (WidthPartitions - 1))
			{
				currentpartition->AddNeighbor(BoundsPartitions[belowindex + LengthPartitions]);

				//Positive X Positive Y Negative Z
				if (col < LengthPartitions - 1)
				{
					currentpartition->AddNeighbor(BoundsPartitions[belowindex + LengthPartitions + 1]);
				}
				//Negative X Positive Y Negative Z
				if (col > 0)
				{
					currentpartition->AddNeighbor(BoundsPartitions[belowindex + LengthPartitions - 1]);
				}

			}
			// Negative Y Negative Z
			if (row > 0)
			{
				currentpartition->AddNeighbor(BoundsPartitions[belowindex - LengthPartitions]);

				//Positive X Negative Y Negative Z
				if (col < LengthPartitions - 1)
				{
					currentpartition->AddNeighbor(BoundsPartitions[belowindex - LengthPartitions + 1]);
				}
				//Negative X Negative Y Negative Z
				if (col > 0)
				{
					currentpartition->AddNeighbor(BoundsPartitions[belowindex - LengthPartitions - 1]);
				}
			}
		}
	}
}

void UBoidBoundsComponent::BeginPlay()
{
	Super::BeginPlay();
	const FString& partitionbasename = TEXT("BoxPartition_");
	FAttachmentTransformRules boxtransformrules = FAttachmentTransformRules::KeepRelativeTransform;
	boxtransformrules.ScaleRule = EAttachmentRule::KeepWorld;

	FVector outextent;
	TArray<FVector> outcenters;
	CalculateBoxCenters(GetComponentLocation(), GetScaledBoxExtent(), LengthPartitions, WidthPartitions, HeightPartitions, outcenters, outextent);

	for (int32 i = 0; i < outcenters.Num(); i++)
	{
		const FName partitionname = FName(partitionbasename + FString::FromInt(i));
		UBoxPartitionComponent * boxbounds = NewObject<UBoxPartitionComponent>(this, partitionname);
		boxbounds->RegisterComponent();
		
		boxbounds->AttachToComponent(this, boxtransformrules);
		boxbounds->SetCollisionResponseToAllChannels(ECR_Ignore);
		boxbounds->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		boxbounds->SetPartitionID(i);
		BoundsPartitions.Emplace(boxbounds);
		boxbounds->SetWorldLocation(outcenters[i]);
		boxbounds->SetBoxExtent(outextent);
		boxbounds->InitBounds();

		const FVector& boxlocation = boxbounds->GetComponentLocation();
		const FVector& boxextent = boxbounds->GetScaledBoxExtent();
	}

	InitializeBoundsNeighbors();
}

void UBoidBoundsComponent::OnRegister()
{
	Super::OnRegister();

	MinBounds = GetComponentLocation() - GetScaledBoxExtent();
	MaxBounds = GetComponentLocation() + GetScaledBoxExtent();

	FractionalPartitionLength = 1 / (GetScaledBoxExtent().X / LengthPartitions * 2.0f);
	FractionalPartitionWidth =  1 / (GetScaledBoxExtent().Y / WidthPartitions * 2.0f);
	FractionalPartitionHeight = 1 / (GetScaledBoxExtent().Z / HeightPartitions * 2.0f);
}

void UBoidBoundsComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const static FName LocationName("RelativeLocation");
	const static FName RotationName("RelativeRotation");
	const static FName ScaleName("RelativeScale3D");
	 
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
