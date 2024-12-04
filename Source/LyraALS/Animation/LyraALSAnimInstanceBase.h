// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Interface/AnimationInterface.h"

#include "LyraALSAnimInstanceBase.generated.h"
/**
 *
 */
UCLASS()
class LYRAALS_API ULyraALSAnimInstanceBase : public UAnimInstance, public IAnimationInterface
{
	GENERATED_BODY()
public:
	virtual void RecieveEquipedGun(EGuns InEquipedGun) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	EGuns EquipedGun;
};
