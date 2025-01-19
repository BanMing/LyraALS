// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

#include "CrouchAnimations.generated.h"

USTRUCT(BlueprintType)
struct FCrouchAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Entry;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Exit;
};