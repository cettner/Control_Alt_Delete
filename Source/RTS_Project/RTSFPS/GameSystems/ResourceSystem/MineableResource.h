// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridAttachmentActor.h"
#include "Interfaces/ResourceGeneratorInterface.h"

#include "NavModifierComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MineableResource.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnResourceNodeDestroyedDelegate)


UCLASS(Blueprintable)
class RTS_PROJECT_API AMineableResource : public AGridAttachmentActor, public IResourceGeneratorInterface, public IRTSObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMineableResource();
	uint32 Mine(uint32 amount_to_mine);


public:
	/*RTSObject Interface Overrides*/
	virtual void SetSelected() override;
	virtual void SetDeselected() override;
	virtual FVector GetUnitLocation() const override;
	virtual void RegisterRTSObject() override;
	virtual void UnRegisterRTSObject() override;
	virtual UTexture* GetThumbnail(const UUserWidget* InDisplayContext = nullptr) const override;
	virtual FName GetUnitName() const override;
	virtual int GetTeam() const override;
	/*******************************/

protected:
	virtual void OnResourcesDepleted();

	public:
		virtual TSubclassOf<UResource> GetResourceClass() const override;

protected:
	/*AActor Override*/
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void PostInitializeComponents() override;

public:
	FOnResourceNodeDestroyedDelegate OnResourceDestroyedDelegate = FOnResourceNodeDestroyedDelegate();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	UDecalSelectionComponent* SelectionComp;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	UStaticMeshComponent* MeshComp = nullptr;

protected:

	UPROPERTY(EditAnywhere, Category = Gameplay)
	bool bisinfinite = false;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	uint32 ResourceVal = 90;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	TSubclassOf<UResource> ResourceClass = UResource::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	UTexture* Thumbnail = nullptr;
};
