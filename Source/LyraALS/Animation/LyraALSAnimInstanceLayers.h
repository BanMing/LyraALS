// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Enums/ELocomotionDirection.h"
#include "Struct/CrouchAnimations.h"
#include "Struct/DirectionalAnimations.h"
#include "Struct/JumpAnimations.h"
#include "Struct/TurnInPlaceAnimations.h"

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
	void SetupStanceTransitionAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

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

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetupPivotAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnPivotUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetupTurnInPlaceEntryState(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetupTurnInPlaceEntryAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateTurnInPlaceEntryAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetupJumpFallLandAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void UpdateJumpFallLandAnims(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pivot")
	FDirectionalAnimations WalkPivotAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pivot")
	FDirectionalAnimations JogPivotAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turn")
	FTurnInPlaceAnimations TurnInPlaceAnimations;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	bool bShouldTurnLeft;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnInPlaceTime;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	TObjectPtr<UAnimSequenceBase> FinalTurnAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crouch")
	FCrouchAnimations CrouchAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crouch")
	FDirectionalAnimations CrouchStartAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crouch")
	FDirectionalAnimations CrouchPivotAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crouch")
	FDirectionalAnimations CrouchDirectionalAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crouch")
	FDirectionalAnimations CrouchStopAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crouch")
	FTurnInPlaceAnimations CrouchTurnInPlaceAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jump")
	FJumpAnimations JumpAnimations;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim Offset")
	TObjectPtr<UBlendSpace> AOStandUp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim Offset")
	TObjectPtr<UBlendSpace> AOCrouch;

protected:
	TObjectPtr<class ULyraALSAnimInstanceBase> BaseAnimInstance;
};
