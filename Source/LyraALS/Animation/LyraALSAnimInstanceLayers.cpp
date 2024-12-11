// Copyright BanMing

#include "Animation/LyraALSAnimInstanceLayers.h"

#include "Animation/AnimExecutionContext.h"
#include "Animation/AnimNodeReference.h"
#include "SequencePlayerLibrary.h"

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
}
