// Copyright BanMing

#pragma once
#include "AnimGraphNode_SkeletalControlBase.h"
#include "LyraALS/Animation/AnimNode/AnimNode_MyCustomControl.h"

#include "AnimGraphNode_MyCustomControl.generated.h"

namespace ENodeTitleType
{
enum Type : int;
}

UCLASS()
class LYRAALSEDITOR_API UAnimGraphNode_MyCustomControl : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_MyCustomControl Node;

public:
	//UAnimGraphNode_MyCustomControl(const FObjectInitializer& ObjectInitializer);

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	// End of UEdGraphNode interface

protected:
	// UAnimGraphNode_SkeletalControlBase interface
	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_MyCustomControl* GetNode() const override
	{
		return &Node;
	}
	// End of UAnimGraphNode_SkeletalControlBase interface
};
