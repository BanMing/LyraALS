// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

#include "SkeletonSockets.generated.h"

USTRUCT(BlueprintType)
struct FSkeletonSockets
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName PistolUnEquipped = "PistolUnEquipped";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RifleUnEquipped = "RifleUnEquipped";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponEquipped = "WeaponEquipped";
};