// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "GateSetting.generated.h"

USTRUCT(BlueprintType)
struct FGateSetting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxWalkSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxAcceleration = 2048.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingDeceleration = 2048.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingFrictionFactor = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingFriction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseSeparateBrakingFriction = false;
};