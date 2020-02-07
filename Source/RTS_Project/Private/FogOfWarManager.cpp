// Fill out your copyright notice in the Description page of Project Settings.

#include "FogOfWarManager.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "FogOfWarWorker.h"
#include "RenderingThread.h"
#include "RTSPlayerController.h"
#include "Engine.h"


AFogOfWarManager::AFogOfWarManager(const FObjectInitializer &FOI) : Super(FOI) {
	
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	textureRegions = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureSize, TextureSize);
	//15 Gaussian samples. Sigma is 2.0.
	//CONSIDER: Calculate the kernel instead, more flexibility...
	blurKernel.Init(0.0f, blurKernelSize);
	blurKernel[0] = 0.000489f;
	blurKernel[1] = 0.002403f;
	blurKernel[2] = 0.009246f;
	blurKernel[3] = 0.02784f;
	blurKernel[4] = 0.065602f;
	blurKernel[5] = 0.120999f;
	blurKernel[6] = 0.174697f;
	blurKernel[7] = 0.197448f;
	blurKernel[8] = 0.174697f;
	blurKernel[9] = 0.120999f;
	blurKernel[10] = 0.065602f;
	blurKernel[11] = 0.02784f;
	blurKernel[12] = 0.009246f;
	blurKernel[13] = 0.002403f;
	blurKernel[14] = 0.000489f;
}

AFogOfWarManager::~AFogOfWarManager() {
	if (FowThread) {
		FowThread->ShutDown();
	}
}

void AFogOfWarManager::BeginPlay() {
	Super::BeginPlay();
	/*
	if (Role != ROLE_Authority && GetWorld())
	{
		ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->FOWManager = this;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%s Spawned!"), *this->GetName()));
		}
	}
	*/
}

void AFogOfWarManager::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (FOWTexture && LastFOWTexture && bHasFOWTextureUpdate && bIsDoneBlending) {
		
		LastFOWTexture->UpdateResource();
		//LastFOWTexture->UpdateTextureRegions((int32)0, (uint32)1, textureRegions, (uint32)(4 * TextureSize), (uint32)4, (uint8*)LastFrameTextureData.GetData(), false);
		UpdateTextureRegions(LastFOWTexture, (int32)0, (uint32)1, textureRegions, (uint32)(4 * TextureSize), (uint32)4, (uint8*)LastFrameTextureData.GetData(), false);

		FOWTexture->UpdateResource();
		//FOWTexture->UpdateTextureRegions((int32)0, (uint32)1, textureRegions, (uint32)(4 * TextureSize), (uint32)4, (uint8*)LastFrameTextureData.GetData(), false);
		UpdateTextureRegions(FOWTexture, (int32)0, (uint32)1, textureRegions, (uint32)(4 * TextureSize), (uint32)4, (uint8*)LastFrameTextureData.GetData(), false);

		bHasFOWTextureUpdate = false;
		bIsDoneBlending = false;
		//Trigger the blueprint update
		OnFowTextureUpdated(FOWTexture, LastFOWTexture);
	}
}

void AFogOfWarManager::StartFOWTextureUpdate() {
	if (!FOWTexture) {
		FOWTexture = UTexture2D::CreateTransient(TextureSize, TextureSize);
		LastFOWTexture = UTexture2D::CreateTransient(TextureSize, TextureSize);
		int arraySize = TextureSize * TextureSize;
		TextureData.Init(FColor(0, 0, 0, 255), arraySize);
		LastFrameTextureData.Init(FColor(0, 0, 0, 255), arraySize);
		HorizontalBlurData.Init(0, arraySize);
		UnfoggedData.Init(false, arraySize);
		FowThread = new AFogOfWarWorker(this);
	}
}

void AFogOfWarManager::OnFowTextureUpdated_Implementation(UTexture2D* currentTexture, UTexture2D* lastTexture) {
	//Handle in blueprint
}

void AFogOfWarManager::RegisterFowActor(AActor* Actor) {
	if (Actor->GetLocalRole() != ROLE_Authority)
	{
		FowActors.AddUnique(Actor);
	}
}

bool AFogOfWarManager::GetIsBlurEnabled() {
	return bIsBlurEnabled;
}

void AFogOfWarManager::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture && Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
			bool bFreeData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;
		RegionData->bFreeData = bFreeData;


		ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)(
			[RegionData](FRHICommandListImmediate& RHICmdList)
			{
				if (RegionData)
				{
					for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
					{
						int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
						if (RegionData->MipIndex >= CurrentFirstMip)
						{
							RHIUpdateTexture2D(
								RegionData->Texture2DResource->GetTexture2DRHI(),
								RegionData->MipIndex - CurrentFirstMip,
								RegionData->Regions[RegionIndex],
								RegionData->SrcPitch,
								RegionData->SrcData
								+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
								+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
							);
						}
					}
					if (RegionData->bFreeData)
					{
						FMemory::Free(RegionData->Regions);
						FMemory::Free(RegionData->SrcData);
					}
					delete RegionData;
				}
			});
	}
}

void AFogOfWarManager::EnableFOW(TArray<AActor *> StartActors)
{
		for (int i = 0; i < StartActors.Num(); i++)
		{
			RegisterFowActor(StartActors[i]);
		}
		IsFoWEnabled = true;
		bIsDoneBlending = true;
		StartFOWTextureUpdate();
		SetActorTickEnabled(true);
}