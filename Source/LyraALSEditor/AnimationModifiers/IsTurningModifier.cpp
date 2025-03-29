// Copyright BanMing

#include "IsTurningModifier.h"

void UIsTurningModifier::OnApply_Implementation(UAnimSequence* Animation)
{
	if (Animation == nullptr)
	{
		UE_LOG(LogAnimation, Error, TEXT("IsTurningModifier failed. Reason: Invalid Animation"));
		return;
	}

	USkeleton* Skeleton = Animation->GetSkeleton();
	if (Skeleton == nullptr)
	{
		UE_LOG(LogAnimation, Error, TEXT("IsTurningModifier failed. Reason: Animation with invalid Skeleton. Animation: %s"), *GetNameSafe(Animation));
		return;
	}

	const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();
	const int32 RootBoneTreeIndex = RefSkeleton.FindBoneIndex("root");
	const float AnimLength = Animation->GetPlayLength();
	const float SampleInterval = 1.f / static_cast<float>(SampleRate);

	FTransform LastFrameRootBoneTrans;
	Animation->GetBoneTransform(LastFrameRootBoneTrans, FSkeletonPoseBoneIndex(RootBoneTreeIndex), AnimLength, true);
	const float LastFrameRootBoneYaw = LastFrameRootBoneTrans.Rotator().Yaw;

	TArray<FRichCurveKey> CurveKeys;
	CurveKeys.AddDefaulted(4);
	float Time = AnimLength;

	// Idle at ending
	CurveKeys[3].Time = AnimLength;
	CurveKeys[3].Value = 0;

	// Find last turning time
	while (Time > 0)
	{
		Time = FMath::Clamp(Time - SampleInterval, 0.f, AnimLength);
		FTransform RootBoneTrans;
		Animation->GetBoneTransform(RootBoneTrans, FSkeletonPoseBoneIndex(RootBoneTreeIndex), Time, true);
		const float RootBoneYaw = RootBoneTrans.Rotator().Yaw;
		if (!FMath::IsNearlyEqual(RootBoneYaw, LastFrameRootBoneYaw))
		{
			break;
		}
	}

	// first idle key
	CurveKeys[2].Time = FMath::Clamp(Time + SampleInterval, 0.f, AnimLength);
	CurveKeys[2].Value = 0;

	// last turning key
	CurveKeys[1].Time = Time;
	CurveKeys[1].Value = 1;

	// Turning at beginning
	CurveKeys[0].Time = 0;
	CurveKeys[0].Value = 1;

	IAnimationDataController& Controller = Animation->GetController();
	Controller.OpenBracket(FText::FromString(TEXT("Setting Motion Curve")));
	const FAnimationCurveIdentifier CurveId = UAnimationCurveIdentifierExtensions::GetCurveIdentifier(Animation->GetSkeleton(), "IsTurning", ERawCurveTrackTypes::RCT_Float);
	Controller.RemoveCurve(CurveId);
	if (CurveKeys.Num() && Controller.AddCurve(CurveId))
	{
		Controller.SetCurveKeys(CurveId, CurveKeys);
	}
	Controller.CloseBracket();
}
