// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGridPlacementCamera.h"
#include "Engine/SCS_Node.h"



void ARTSGridPlacementCamera::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PlacementActor = CreatePlacementActor(StructureClass);
}

AGridAttatchmentActor * ARTSGridPlacementCamera::CreatePlacementActor(const TSubclassOf<AActor> InActorClass) const
{
	UWorld * World = GetWorld();
	if (World == nullptr) return nullptr;

	/*Check the default actor for the class, find the first component that contains a mesh if there's no mesh we have nothing to place in the world*/
	/*TODO: Find a way so it grabs the "Correct" mesh and not just the first one*/
	UMeshComponent * defaultprimitive = Cast<UMeshComponent>(FindDefaultComponentByClass(InActorClass, UMeshComponent::StaticClass()));
	if (defaultprimitive == nullptr) return nullptr;

	AGridAttatchmentActor * placeactor = nullptr;

	/**/
	UStaticMeshComponent * staticprimitive = Cast<UStaticMeshComponent>(defaultprimitive);
	if (staticprimitive)
	{
		placeactor = World->SpawnActor<AGridAttatchmentActor>(GridActorClass);
		
		UActorComponent * newcomp = placeactor->AddComponentByClass(staticprimitive->GetClass(), false, staticprimitive->GetRelativeTransform(), false);
		UStaticMeshComponent * staticcomp = Cast<UStaticMeshComponent>(newcomp);
		
		if (staticcomp)
		{
			staticcomp->SetStaticMesh(staticprimitive->GetStaticMesh());
		}
	}
	else
	{
		USkeletalMeshComponent * skeletalprimitive = Cast<USkeletalMeshComponent>(defaultprimitive);
		if (skeletalprimitive)
		{
			placeactor = World->SpawnActor<AGridAttatchmentActor>(GridActorClass);
			UActorComponent * newcomp = placeactor->AddComponentByClass(skeletalprimitive->GetClass(), false, skeletalprimitive->GetRelativeTransform(), false);
			USkeletalMeshComponent * skeletalcomp = Cast<USkeletalMeshComponent>(newcomp);

			if (skeletalcomp)
			{
				skeletalcomp->SetSkeletalMesh(skeletalprimitive->SkeletalMesh);
			}
		}
	}

	return(placeactor);
}

AGridAttatchmentActor * ARTSGridPlacementCamera::GetPlacementActor() const
{
	return PlacementActor;
}

bool ARTSGridPlacementCamera::IsPlacingActor() const
{
	return (PlacementActor != nullptr);
}

ASquareGameGrid * ARTSGridPlacementCamera::GetCurrentGrid() const
{
	if (PlacementActor == nullptr) return nullptr;

	return(PlacementActor->GetParentGrid());

}

UActorComponent * ARTSGridPlacementCamera::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass) const
{
	if (!IsValid(InActorClass))
	{
		return nullptr;
	}

	// Check CDO.
	AActor* ActorCDO = InActorClass->GetDefaultObject<AActor>();
	UActorComponent* FoundComponent = ActorCDO->FindComponentByClass(InComponentClass);

	if (FoundComponent != nullptr)
	{
		return FoundComponent;
	}

	// Check blueprint nodes. Components added in blueprint editor only (and not in code) are not available from
	// CDO.
	UBlueprintGeneratedClass* RootBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(InActorClass);
	UClass* ActorClass = InActorClass;

	// Go down the inheritance tree to find nodes that were added to parent blueprints of our blueprint graph.
	do
	{
		UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(ActorClass);
		if (!ActorBlueprintGeneratedClass)
		{
			return nullptr;
		}

		const TArray<USCS_Node*>& ActorBlueprintNodes = ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

		for (USCS_Node* Node : ActorBlueprintNodes)
		{
			if (Node->ComponentClass->IsChildOf(InComponentClass))
			{
				return Node->GetActualComponentTemplate(RootBlueprintGeneratedClass);
			}
		}

		ActorClass = Cast<UClass>(ActorClass->GetSuperStruct());

	} while (ActorClass != AActor::StaticClass());

	return nullptr;
}

void ARTSGridPlacementCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsPlacingActor()) return;

	APlayerController * pc = GetController<APlayerController>();
	if (pc == nullptr) return;

	FHitResult hit;
	pc->GetHitResultUnderCursor(GRID_CHANNEL, false, hit);

	
	ASquareGameGrid * currentgrid = GetCurrentGrid();

	FGridTile hittile = currentgrid->GetTileFromLocation(hit.Location);
	if (hittile.IsValid && hit.bBlockingHit)
	{
		PlacementActor->SetTileLocation(hittile);
	}
}
