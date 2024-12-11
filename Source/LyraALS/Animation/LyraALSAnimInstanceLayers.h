// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Struct/DirectionalAnimations.h"

#include "LyraALSAnimInstanceLayers.generated.h"

class UAnimSequenceBase;
struct FAnimUpdateContext;
struct FAnimNodeReference;
/**
 *
 */
UCLASS()
class LYRAALS_API ULyraALSAnimInstanceLayers : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnIdleUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnCycleUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> IdleAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDirectionalAnimations WalkDirectionalAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDirectionalAnimations JogDirectionalAnimations;
};
