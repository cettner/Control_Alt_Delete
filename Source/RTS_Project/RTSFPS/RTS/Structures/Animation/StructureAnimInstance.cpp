// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureAnimInstance.h"
#include "..\InterFaces\BuildableInterface.h"




void UStructureAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	/*Structure wont go back to being unbuilt after building, so only update if we're unbuilt*/
	if (!bIsBuilt)
	{
		AActor* actorowner = GetOwningActor();
		IBuildableInterface* buildable = Cast<IBuildableInterface>(actorowner);
		if (buildable != nullptr)
		{
			bIsBuilt = buildable->IsConstructed();
		}
	}

}