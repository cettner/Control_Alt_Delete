#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryDebugHelpers.h"

#if WITH_GAMEPLAY_DEBUGGER_MENU
#include "GameplayDebuggerCategory.h"

class AActor;
class APlayerController;

class FGameplayDebuggerCategory_Order : public FGameplayDebuggerCategory
{

public:
	FGameplayDebuggerCategory_Order();
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;
	virtual FDebugRenderSceneProxy* CreateDebugSceneProxy(const UPrimitiveComponent* InComponent, FDebugDrawDelegateHelper*& OutDelegateHelper) override;
	virtual void OnDataPackReplicated(int32 DataPackId) override;
	RTS_PROJECT_API static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	TMap<FString, FVector> VectorPropertyMap = TMap<FString, FVector>();
	TMap<FString, int32> IntegerPropertyMap = TMap<FString, int32>();
	TMap<FString, FString> StringPropertyMap = TMap<FString, FString>();


#if USE_EQS_DEBUGGER
protected:
	struct FOrderQueryRepData
	{
		TArray<EQSDebug::FQueryData> QueryDebugData;

		void Serialize(FArchive& Ar);
	};
	FOrderQueryRepData DataPack;

	int32 DrawLookedAtItem(const EQSDebug::FQueryData& QueryData, APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) const;
	void DrawDetailedItemTable(const EQSDebug::FQueryData& QueryData, int32 LookedAtItemIndex, FGameplayDebuggerCanvasContext& CanvasContext) const;
	void DrawDetailedItemRow(const EQSDebug::FItemData& ItemData, const TArray<uint8>& TestRelevancy, float MaxScore, FGameplayDebuggerCanvasContext& CanvasContext) const;
#endif


protected:
	uint32 bDrawLabels : 1;
	uint32 bDrawFailedItems : 1;
	uint32 bShowDetails : 1;

	int32 MaxItemTableRows;
	int32 MaxQueries;
	int32 ShownQueryIndex;
};

#endif // WITH_GAMEPLAY_DEBUGGER_MENU
