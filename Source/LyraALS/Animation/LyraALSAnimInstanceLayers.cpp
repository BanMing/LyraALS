// Copyright BanMing

#include "Animation/LyraALSAnimInstanceLayers.h"

#include "AnimCharacterMovementLibrary.h"
#include "AnimDistanceMatchingLibrary.h"
#include "Animation/AnimExecutionContext.h"
#include "Animation/AnimNodeReference.h"
#include "Animation/LyraALSAnimInstanceBase.h"
#include "Enums/EGate.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SequenceEvaluatorLibrary.h"
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

void ULyraALSAnimInstanceLayers::SetupStopAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	UAnimSequenceBase* Sequence;
	if (GetBaseAnimInstance()->GetCurrentGate() == EGate::Jogging)
	{
		Sequence = GetSequence(JogStopAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}
	else
	{
		Sequence = GetSequence(WalkStopAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}
	USequenceEvaluatorLibrary::SetSequence(SequenceEvaluatorReference, Sequence);
	USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, 0.f);
}

void ULyraALSAnimInstanceLayers::OnStopUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	const UCharacterMovementComponent* CharacterMovementComp = GetBaseAnimInstance()->GetCharacterMovementComp();
	const FVector StopLocation = UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(GetBaseAnimInstance()->GetCharacterVelocity2D(), CharacterMovementComp->bUseSeparateBrakingFriction,
		CharacterMovementComp->BrakingFriction, CharacterMovementComp->GroundFriction, CharacterMovementComp->BrakingDecelerationWalking, CharacterMovementComp->BrakingDecelerationWalking);
	const float StopDistance = StopLocation.Size2D();
	if (StopDistance > 0.f)
	{
		UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluatorReference, StopDistance, "Distance");
	}
	else
	{
		USequenceEvaluatorLibrary::AdvanceTime(UpdateContext, SequenceEvaluatorReference);
	}
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
