// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Enums/ELocomotionDirection.h"
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
public:
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;

	// Native Uninitialize override point
	virtual void NativeUninitializeAnimation() override;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	ULyraALSAnimInstanceBase* GetBaseAnimInstance();

protected:
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnIdleUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnCycleUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetupStopAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnStopUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetupStartAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnStartUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

private:
	UAnimSequenceBase* GetSequence(const FDirectionalAnimations& DirectionalAnimations, ELocomotionDirection LocomotionDirection);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Idle")
	TObjectPtr<UAnimSequenceBase> IdleAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cycle")
	FDirectionalAnimations WalkDirectionalAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cycle")
	FDirectionalAnimations JogDirectionalAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stop")
	FDirectionalAnimations WalkStopAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stop")
	FDirectionalAnimations JogStopAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Start")
	FDirectionalAnimations WalkStartAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Start")
	FDirectionalAnimations JogStartAnimations;

protected:
	TObjectPtr<class ULyraALSAnimInstanceBase> BaseAnimInstance;
};
