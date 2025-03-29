// Copyright BanMing

#include "AnimGraphNode_MyCustomControl.h"

#define LOCTEXT_NAMESPACE "AnimGraphNode_MyCustomControl"

UAnimGraphNode_MyCustomControl::UAnimGraphNode_MyCustomControl(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FText UAnimGraphNode_MyCustomControl::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GetControllerDescription();
}

FText UAnimGraphNode_MyCustomControl::GetTooltipText() const
{
	return FText::FromString("My Custom Control Tooltip");
}

FLinearColor UAnimGraphNode_MyCustomControl::GetNodeTitleColor() const
{
	return FLinearColor::Blue;
}

FText UAnimGraphNode_MyCustomControl::GetControllerDescription() const
{
	return FText::FromString("My Custom Control");
}
#undef LOCTEXT_NAMESPACE