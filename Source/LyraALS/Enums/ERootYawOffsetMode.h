// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	Accumulate,
	BlendOut,
	Hold
};
