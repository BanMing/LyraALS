// Copyright BanMing
#include "AnimNode_MyCustomControl.h"

#include "Animation/AnimInstanceProxy.h"

void FAnimNode_MyCustomControl::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	// 获取当前骨骼的 Component Space Transform
	const FCompactPoseBoneIndex BoneIndex = TargetBone.GetCompactPoseIndex(Output.Pose.GetPose().GetBoneContainer());
	FTransform BoneTransform = Output.Pose.GetComponentSpaceTransform(BoneIndex);

	// 修改 Transform（示例：让骨骼沿Z轴偏移）
	const float WiggleOffset = FMath::Sin(DeltaTime * Speed);
	BoneTransform.AddToTranslation(FVector(0.f, 0.f, WiggleOffset));

	// 写入输出
	OutBoneTransforms.Add(FBoneTransform(BoneIndex, BoneTransform));
}

void FAnimNode_MyCustomControl::UpdateInternal(const FAnimationUpdateContext& Context)
{
	FAnimNode_SkeletalControlBase::UpdateInternal(Context);
	DeltaTime = Context.GetDeltaTime();
}

bool FAnimNode_MyCustomControl::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return TargetBone.IsValidToEvaluate(RequiredBones);
}

void FAnimNode_MyCustomControl::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	FAnimNode_SkeletalControlBase::Initialize_AnyThread(Context);
	TargetBone.Initialize(Context.AnimInstanceProxy->GetRequiredBones());
}
