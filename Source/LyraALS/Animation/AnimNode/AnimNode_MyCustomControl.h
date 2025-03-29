// Copyright BanMing

#pragma once

#include "BoneControllers/AnimNode_SkeletalControlBase.h"

#include "AnimNode_MyCustomControl.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct LYRAALS_API FAnimNode_MyCustomControl : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

private:
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;

public:
	UPROPERTY(EditAnywhere, Category = "Settings")
	FBoneReference TargetBone;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Speed = 1.0f;

private:
	float DeltaTime;
};
