// Copyright BanMing

#include "Animation/LyraALSAnimInstanceLayers.h"

#include "Animation/AnimExecutionContext.h"
#include "Animation/AnimNodeReference.h"
#include "Animation/LyraALSAnimInstanceBase.h"
#include "Enums/EGate.h"
#include "SequencePlayerLibrary.h"

void ULyraALSAnimInstanceLayers::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	GetBaseAnimInstance();
}

void ULyraALSAnimInstanceLayers::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
	BaseAnimInstance = nullptr;
}

ULyraALSAnimInstanceBase* ULyraALSAnimInstanceLayers::GetBaseAnimInstance()
{
	if (BaseAnimInstance == nullptr)
	{
		BaseAnimInstance = Cast<ULyraALSAnimInstanceBase>(GetOwningComponent()->GetAnimInstance());
	}
	return BaseAnimInstance;
}

void ULyraALSAnimInstanceLayers::OnIdleUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed)
	{
		return;
	}

	USequencePlayerLibrary::SetSequenceWithInertialBlending(UpdateContext, SequencePlayerReference, IdleAnim);
}

void ULyraALSAnimInstanceLayers::OnCycleUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	UAnimSequenceBase* Sequence;
	if (GetBaseAnimInstance()->GetCurrentGate() == EGate::Jogging)
	{
		Sequence = GetSequence(JogDirectionalAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}
	else
	{
		Sequence = GetSequence(WalkDirectionalAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}

	USequencePlayerLibrary::SetSequenceWithInertialBlending(UpdateContext, SequencePlayerReference, Sequence);
}

UAnimSequenceBase* ULyraALSAnimInstanceLayers::GetSequence(const FDirectionalAnimations& DirectionalAnimations, ELocomotionDirection LocomotionDirection)
{
	switch (LocomotionDirection)
	{
		case ELocomotionDirection::Forward:
			return DirectionalAnimations.Forward;
		case ELocomotionDirection::Backward:
			return DirectionalAnimations.Backward;
		case ELocomotionDirection::Left:
			return DirectionalAnimations.Left;
		case ELocomotionDirection::Right:
			return DirectionalAnimations.Right;
	}
	return DirectionalAnimations.Forward;
}
