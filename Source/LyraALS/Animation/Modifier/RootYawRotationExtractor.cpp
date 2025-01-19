// Copyright BanMing
#include "Animation/Modifier/RootYawRotationExtractor.h"

#include "MotionExtractorUtilities.h"

void URootYawRotationExtractor::OnApply_Implementation(UAnimSequence* Animation)
{
	if (Animation == nullptr)
	{
		UE_LOG(LogAnimation, Error, TEXT("RootYawRotationExtractor failed. Reason: Invalid Animation"));
		return;
	}

	USkeleton* Skeleton = Animation->GetSkeleton();
	if (Skeleton == nullptr)
	{
		UE_LOG(LogAnimation, Error, TEXT("RootYawRotationExtractor failed. Reason: Animation with invalid Skeleton. Animation: %s"), *GetNameSafe(Animation));
		return;
	}

	const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();
	const int32 RootBoneTreeIndex = RefSkeleton.FindBoneIndex("root");
	const float AnimLength = Animation->GetPlayLength();
	const float SampleInterval = 1.f / static_cast<float>(SampleRate);

	TArray<FRichCurveKey> CurveKeys;
	float Time = 0.f;

	while (Time < AnimLength)
	{
		Time = FMath::Clamp(Time + SampleInterval, 0.f, AnimLength);
		FTransform RootBoneTrans;
		Animation->GetBoneTransform(RootBoneTrans, FSkeletonPoseBoneIndex(RootBoneTreeIndex), Time, true);
		FRichCurveKey& Key = CurveKeys.AddDefaulted_GetRef();
		Key.Time = Time;
		// we need recover the yaw rotation. So here * -1?????????
		Key.Value = -RootBoneTrans.Rotator().Yaw;
	}

	// Keep the last series values to 0. In order to use easy way to rotate character
	const float LastKeyValue = CurveKeys[CurveKeys.Num() - 1].Value;
	if (!FMath::IsNearlyZero(LastKeyValue))
	{
		for (FRichCurveKey& Key : CurveKeys)
		{
			Key.Value -= LastKeyValue;
		}
	}

	IAnimationDataController& Controller = Animation->GetController();
	Controller.OpenBracket(FText::FromString(TEXT("Setting Motion Curve")));
	const FAnimationCurveIdentifier CurveId = UAnimationCurveIdentifierExtensions::GetCurveIdentifier(Animation->GetSkeleton(), "root_rotation_Z", ERawCurveTrackTypes::RCT_Float);
	Controller.RemoveCurve(CurveId);
	if (CurveKeys.Num() && Controller.AddCurve(CurveId))
	{
		Controller.SetCurveKeys(CurveId, CurveKeys);
	}
	Controller.CloseBracket();
}
