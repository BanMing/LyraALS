// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

#include "TurnInPlaceAnimations.generated.h"

USTRUCT(BlueprintType)
struct FTurnInPlaceAnimations
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Right90;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Left90;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Right180;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> Left180;
};