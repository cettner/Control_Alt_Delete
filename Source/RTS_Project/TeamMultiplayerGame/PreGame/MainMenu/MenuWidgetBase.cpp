#include "MenuWidgetBase.h"




bool UMenuWidgetBase::Initialize()
{
	bool retval = Super::Initialize();
	InitMenuBindings();
	return(retval);
}


void UMenuWidgetBase::InitMenuBindings()
{
}

TArray<FMenuClickBindingInfo> UMenuWidgetBase::GetMenuBindings() const
{
	return BindingInfo;
}