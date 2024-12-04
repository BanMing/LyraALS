// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Enums/EGuns.h"
#include "GameFramework/Character.h"

#include "LyraALSCharacterBase.generated.h"

class IAnimationInterface;

UCLASS()
class LYRAALS_API ALyraALSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALyraALSCharacterBase();

	void SwitchGun(EGuns Gun);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	EGuns EquipedGun = EGuns::UnArmed;

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IAnimationInterface> AnimationInterface;
};
