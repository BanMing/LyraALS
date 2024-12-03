// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Interface/AnimationInterface.h"

#include "AnimInstanceBase.generated.h"
/**
 *
 */
UCLASS()
class LYRAALS_API UAnimInstanceBase : public UAnimInstance, public IAnimationInterface
{
	GENERATED_BODY()
public:
	// Start IAnimationInterface
	virtual void RecieveMaxWalkSpeed_Implementation(float MaxWalkSpeed) override;
	// End IAnimationInterface
};
