// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacementActor.h"
#include "Engine/SCS_Node.h"

// Sets default values
APlacementActor::APlacementActor() : Super()
{
	MeshComp = CreateDefaultSubobject<UMeshComponent>(TEXT("DefaultMesh"));
}

void APlacementActor::SetMeshComp(UMeshComponent* InMeshComp)
{
	MeshComp = InMeshComp;
}

void APlacementActor::PreInitializeGridActor(AGridAttatchmentActor * GridActor, const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform)
{

	/*Check the default actor for the class, find the first component that contains a mesh if there's no mesh we have nothing to place in the world*/
    /*TODO: Find a way so it grabs the "Correct" mesh and not just the first one*/
	UMeshComponent * defaultprimitive = Cast<UMeshComponent>(FindDefaultComponentByClass(InActorClass, UMeshComponent::StaticClass()));
	if (defaultprimitive == nullptr) return;

	MeshComp = Cast<UMeshComponent>(this->AddComponentByClass(defaultprimitive->GetClass(), false, defaultprimitive->GetRelativeTransform(), false));
	
}

UActorComponent * APlacementActor::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass) const
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

UMeshComponent * APlacementActor::GetMesh() const
{
	return MeshComp;
}
