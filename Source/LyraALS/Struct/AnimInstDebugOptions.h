// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

#include "AnimInstDebugOptions.generated.h"

USTRUCT(BlueprintType)
struct FAnimInstDebugOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bShowLocomotionData = false;

	UPROPERTY(EditAnywhere)
	bool bShowGateData = false;

	UPROPERTY(EditAnywhere)
	bool bShowDistanceMatching = false;
};
