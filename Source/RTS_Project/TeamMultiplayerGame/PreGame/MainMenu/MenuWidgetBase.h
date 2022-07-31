#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include"MenuWidgetBase.generated.h"


struct FMenuClickBindingInfo
{
	TSubclassOf<UWidget> BindToClass = nullptr;
	UButton * BindingButton = nullptr;
};

UCLASS()
class RTS_PROJECT_API UMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TArray<FMenuClickBindingInfo> GetMenuBindings() const;

protected:
	virtual bool Initialize() override;

protected:
	virtual void InitMenuBindings();


protected:
	TArray<FMenuClickBindingInfo> BindingInfo = TArray<FMenuClickBindingInfo>();

};