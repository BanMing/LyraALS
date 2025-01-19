// Copyright BanMing

#pragma once

#include "AnimationModifier.h"
#include "CoreMinimal.h"

#include "IsTurningModifier.generated.h"

/**
 *
 */
UCLASS()
class LYRAALS_API UIsTurningModifier : public UAnimationModifier
{
	GENERATED_BODY()

public:
	virtual void OnApply_Implementation(UAnimSequence* Animation) override;

public:
	/** Rate used to sample the animation */
	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = "1"))
	int32 SampleRate = 30;
};
