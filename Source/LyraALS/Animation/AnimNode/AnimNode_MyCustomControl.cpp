// Copyright BanMing
#include "AnimNode_MyCustomControl.h"

#include "Animation/AnimInstanceProxy.h"

void FAnimNode_MyCustomControl::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	// ��ȡ��ǰ������ Component Space Transform
	const FCompactPoseBoneIndex BoneIndex = TargetBone.GetCompactPoseIndex(Output.Pose.GetPose().GetBoneContainer());
	FTransform BoneTransform = Output.Pose.GetComponentSpaceTransform(BoneIndex);

	// �޸� Transform��ʾ�����ù�����Z��ƫ�ƣ�
	const float WiggleOffset = FMath::Sin(DeltaTime * Speed);
	BoneTransform.AddToTranslation(FVector(0.f, 0.f, WiggleOffset));

	// д�����
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
