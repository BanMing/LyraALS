// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(BlueprintNativeEvent)
	void RecieveMaxWalkSpeed(float MaxWalkSpeed);

	UFUNCTION(BlueprintNativeEvent)
	void RecieveVelocity();
};
