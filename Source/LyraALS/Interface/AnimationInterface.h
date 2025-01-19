// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Enums/EGate.h"
#include "Enums/EGuns.h"
#include "UObject/Interface.h"

#include "AnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LYRAALS_API IAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReceiveEquippedGun(EGuns InEquipedGun) = 0;
	virtual void ReceiveCurrentGate(EGate InGate) = 0;
	virtual void ReceiveGroundDistance(float InGroundDistance) = 0;
};
