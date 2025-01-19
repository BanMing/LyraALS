// Copyright BanMing

#include "Animation/LyraALSAnimInstanceLayers.h"

#include "AnimCharacterMovementLibrary.h"
#include "AnimDistanceMatchingLibrary.h"
#include "Animation/AnimExecutionContext.h"
#include "Animation/AnimNodeReference.h"
#include "Animation/LyraALSAnimInstanceBase.h"
#include "Enums/EGate.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

void ULyraALSAnimInstanceLayers::SetupStanceTransitionAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed)
	{
		return;
	}
	const ULyraALSAnimInstanceBase* AnimationBase = GetBaseAnimInstance();

	// If it is called this function, the AnimationBase->GetIsCrouchStateChanged() is always True
	UAnimSequenceBase* Sequence = CrouchAnimations.Exit;
	if (AnimationBase->GetIsCrouching())
	{
		Sequence = CrouchAnimations.Entry;
	}

	USequencePlayerLibrary::SetSequenceWithInertialBlending(UpdateContext, SequencePlayerReference, Sequence);
}

void ULyraALSAnimInstanceLayers::OnIdleUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequencePlayerReference SequencePlayerReference = USequencePlayerLibrary::ConvertToSequencePlayer(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed)
	{
		return;
	}
	UAnimSequenceBase* Sequence = GetBaseAnimInstance()->GetIsCrouching() ? CrouchAnimations.Idle : IdleAnim;
	USequencePlayerLibrary::SetSequenceWithInertialBlending(UpdateContext, SequencePlayerReference, Sequence);
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
	else if (GetBaseAnimInstance()->GetCurrentGate() == EGate::Walking)
	{
		Sequence = GetSequence(WalkDirectionalAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}
	else
	{
		Sequence = GetSequence(CrouchDirectionalAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
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
	else if (GetBaseAnimInstance()->GetCurrentGate() == EGate::Walking)
	{
		Sequence = GetSequence(WalkStopAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}
	else
	{
		Sequence = GetSequence(CrouchStopAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
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

void ULyraALSAnimInstanceLayers::SetupStartAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
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
		Sequence = GetSequence(JogStartAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}
	else if (GetBaseAnimInstance()->GetCurrentGate() == EGate::Walking)
	{
		Sequence = GetSequence(WalkStartAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}
	else
	{
		Sequence = GetSequence(CrouchStartAnimations, GetBaseAnimInstance()->GetLocomotionDirection());
	}

	USequenceEvaluatorLibrary::SetSequence(SequenceEvaluatorReference, Sequence);
	USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, 0.f);
}

void ULyraALSAnimInstanceLayers::OnStartUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}
	UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(UpdateContext, SequenceEvaluatorReference, GetBaseAnimInstance()->GetDeltaLocation(), "Distance");
}

void ULyraALSAnimInstanceLayers::SetupPivotAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	GetBaseAnimInstance()->SetPivotAcceleration2D(GetBaseAnimInstance()->GetCurAcceleration2D());

	UAnimSequenceBase* Sequence;
	if (GetBaseAnimInstance()->GetCurrentGate() == EGate::Jogging)
	{
		Sequence = GetSequence(JogPivotAnimations, GetBaseAnimInstance()->GetAccelerationLocomotionDirection());
	}
	else if (GetBaseAnimInstance()->GetCurrentGate() == EGate::Walking)
	{
		Sequence = GetSequence(WalkPivotAnimations, GetBaseAnimInstance()->GetAccelerationLocomotionDirection());
	}
	else
	{
		Sequence = GetSequence(CrouchPivotAnimations, GetBaseAnimInstance()->GetAccelerationLocomotionDirection());
	}

	USequenceEvaluatorLibrary::SetSequence(SequenceEvaluatorReference, Sequence);
	USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, 0.f);
}

void ULyraALSAnimInstanceLayers::OnPivotUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	const ULyraALSAnimInstanceBase* AnimationBase = GetBaseAnimInstance();
	const FVector Velocity2DNormal = AnimationBase->GetCharacterVelocity2D().GetSafeNormal();
	const FVector Acceleration2DNormal = AnimationBase->GetCurAcceleration2D().GetSafeNormal();
	const bool bIsSameDir = Velocity2DNormal.Dot(Acceleration2DNormal) > 0;
	if (bIsSameDir)
	{
		UAnimDistanceMatchingLibrary::AdvanceTimeByDistanceMatching(UpdateContext, SequenceEvaluatorReference, GetBaseAnimInstance()->GetDeltaLocation(), "Distance");
	}
	else
	{
		// should check the animation is right?
		const UCharacterMovementComponent* CharacterMovementComp = GetBaseAnimInstance()->GetCharacterMovementComp();
		const FVector PivotLocation =
			UAnimCharacterMovementLibrary::PredictGroundMovementPivotLocation(AnimationBase->GetCurAcceleration2D(), AnimationBase->GetCharacterVelocity2D(), CharacterMovementComp->GroundFriction);
		UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluatorReference, PivotLocation.Size2D(), "Distance");
	}
}

void ULyraALSAnimInstanceLayers::SetupTurnInPlaceEntryState(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	bShouldTurnLeft = GetBaseAnimInstance()->GetRootYawOffset() > 0;
}

void ULyraALSAnimInstanceLayers::SetupTurnInPlaceEntryAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	const ULyraALSAnimInstanceBase* AnimationBase = GetBaseAnimInstance();
	const float RootYawOffset = FMath::Abs(AnimationBase->GetRootYawOffset());
	FTurnInPlaceAnimations TurnInPlaceAnims = AnimationBase->GetIsCrouching() ? CrouchTurnInPlaceAnimations : TurnInPlaceAnimations;
	if (RootYawOffset > 90.f && RootYawOffset < 180.f)
	{
		FinalTurnAnimation = bShouldTurnLeft ? TurnInPlaceAnims.Left180 : TurnInPlaceAnims.Right180;
	}
	else
	{
		FinalTurnAnimation = bShouldTurnLeft ? TurnInPlaceAnims.Left90 : TurnInPlaceAnims.Right90;
	}
	TurnInPlaceTime = 0.f;
	USequenceEvaluatorLibrary::SetSequence(SequenceEvaluatorReference, FinalTurnAnimation);
	USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, TurnInPlaceTime);
}

void ULyraALSAnimInstanceLayers::UpdateTurnInPlaceEntryAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	const ULyraALSAnimInstanceBase* AnimationBase = GetBaseAnimInstance();
	const float DeltaTime = UpdateContext.GetContext()->GetDeltaTime();
	TurnInPlaceTime += DeltaTime;
	USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, TurnInPlaceTime);
}

void ULyraALSAnimInstanceLayers::SetupJumpFallLandAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	USequenceEvaluatorLibrary::SetExplicitTime(SequenceEvaluatorReference, 0.f);
}

void ULyraALSAnimInstanceLayers::UpdateJumpFallLandAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node)
{
	EAnimNodeReferenceConversionResult Result;
	FSequenceEvaluatorReference SequenceEvaluatorReference = USequenceEvaluatorLibrary::ConvertToSequenceEvaluator(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed || GetBaseAnimInstance() == nullptr)
	{
		return;
	}

	const ULyraALSAnimInstanceBase* AnimationBase = GetBaseAnimInstance();
	UAnimDistanceMatchingLibrary::DistanceMatchToTarget(SequenceEvaluatorReference, AnimationBase->GetIncomingGroundDistance(), "Distance");
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
