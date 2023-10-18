// Fill out your copyright notice in the Description page of Project Settings.


#include "GridAttachmentActor.h"

#include "EngineUtils.h"

// Sets default values
AGridAttachmentActor::AGridAttachmentActor()
{
	PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));

    GridLayerComp = CreateDefaultSubobject<UGridLayerComponent>(TEXT("GridLayerComp"));
    GridLayerComp->SetupAttachment(RootComponent);
}

void AGridAttachmentActor::SetRootTile(UGridTile* InRootTile)
{
    RootTile = InRootTile;

    if (IsValid(RootTile))
    {
        const FVector tilelocation = InRootTile->GetTileCenter();
        SetActorLocation(tilelocation);
    }
}

AGameGrid * AGridAttachmentActor::GetGameGrid() const
{
    AGameGrid* retval = nullptr;
    if (IsValid(RootTile))
    {
        retval = RootTile->GetGameGrid();
    }
    return retval;
}

void AGridAttachmentActor::PreInitializeComponents()
{
    Super::PreInitializeComponents();

    AGameGrid* foundgrid = FindGrid(GetWorld());

    if (IsValid(foundgrid))
    {
        UGridTile* mytile = foundgrid->GetTileFromLocation(GetActorLocation());

        if (IsValid(mytile))
        {
            SetRootTile(mytile);
        }
    }
}

void AGridAttachmentActor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

     TInlineComponentArray<UGridLayerComponent*> layercomps = TInlineComponentArray<UGridLayerComponent*>();
    GetComponents<UGridLayerComponent>(layercomps);

    for (int32 i = 0; i < layercomps.Num(); i++)
    {
        if (IsValid(layercomps[i]))
        {
            layercomps[i]->ApplyLayers();
        }
    }
}

AGameGrid* AGridAttachmentActor::FindGrid(UWorld* InWorld)
{
    AGameGrid* retval = nullptr;

    if (IsValid(InWorld))
    {
        for (TActorIterator<AGameGrid> ActorItr(InWorld); ActorItr; ++ActorItr)
        {
            AGameGrid* found = *ActorItr;

            if (found->IsValidGridLocation(GetActorLocation()))
               {
                  retval = found;
                  break;
               }
        }
    }

    return retval;
}

#if WITH_EDITOR

static FName Name_RelativeLocation = USceneComponent::GetRelativeLocationPropertyName();
static FName Name_RelativeRotation = USceneComponent::GetRelativeRotationPropertyName();
static FName Name_RelativeScale3D = USceneComponent::GetRelativeScale3DPropertyName();

void AGridAttachmentActor::OnGridConstructed(AGameGrid* InGrid)
{
    if (IsValid(InGrid))
    {
        UGridTile* mytile = InGrid->GetTileFromLocation(GetActorLocation());

        if (IsValid(mytile) && (mytile != RootTile))
        {
            SetRootTile(mytile);
            PostRootTileChanged();

        }

        TInlineComponentArray<UGridLayerComponent*> layercomps = TInlineComponentArray<UGridLayerComponent*>();
        GetComponents<UGridLayerComponent>(layercomps);

        for (int32 i = 0; i < layercomps.Num(); i++)
        {
            if (IsValid(layercomps[i]))
            {
                layercomps[i]->ApplyLayers();
            }
        }
    }
}

void AGridAttachmentActor::PostRootTileChanged()
{
    TInlineComponentArray<UGridLayerComponent*> layercomps = TInlineComponentArray<UGridLayerComponent*>();
    GetComponents<UGridLayerComponent>(layercomps);

    for (int32 i = 0; i < layercomps.Num(); i++)
    {
        if (IsValid(layercomps[i]))
        {
            layercomps[i]->PostRootTileChanged();
        }
    }
}

void AGridAttachmentActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FProperty* MemberPropertyThatChanged = PropertyChangedEvent.MemberProperty;
    const FName MemberPropertyName = MemberPropertyThatChanged != NULL ? MemberPropertyThatChanged->GetFName() : NAME_None;

    const bool bTransformationChanged = (MemberPropertyName == Name_RelativeLocation || MemberPropertyName == Name_RelativeRotation || MemberPropertyName == Name_RelativeScale3D);

    if (bTransformationChanged)
    {
        AGameGrid* foundgrid = FindGrid(GetWorld());

        if (IsValid(foundgrid))
        {
            UGridTile* prevtile = GetRootTile();
            UGridTile* mytile = foundgrid->GetTileFromLocation(GetActorLocation());

            if (IsValid(mytile))
            {
                SetRootTile(mytile);
            }

            if (prevtile != mytile)
            {
                PostRootTileChanged();
            }
        }
    }
}

void AGridAttachmentActor::PostEditMove(bool bFinished)
{
    Super::PostEditMove(bFinished);

    if (bFinished == true)
    {
        UGridTile* prevtile = GetRootTile();

        AGameGrid* foundgrid = FindGrid(GetWorld());

        if (IsValid(foundgrid))
        {
            UGridTile* mytile = foundgrid->GetTileFromLocation(GetActorLocation());

            if (IsValid(mytile))
            {
                SetRootTile(mytile);
            }

            if (prevtile != mytile)
            {
                PostRootTileChanged();
            }
        }
    }
}
#endif

