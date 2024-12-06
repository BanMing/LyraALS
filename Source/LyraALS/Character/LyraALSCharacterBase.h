// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Enums/EGuns.h"
#include "GameFramework/Character.h"

#include "LyraALSCharacterBase.generated.h"

class IAnimationInterface;
class UAnimInstance;

UCLASS()
class LYRAALS_API ALyraALSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALyraALSCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	void SwitchGun(EGuns Gun);

protected:
	UPROPERTY(BlueprintReadOnly)
	EGuns EquippedGun = EGuns::UnArmed;

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IAnimationInterface> AnimationInterface;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Layers")
	TSubclassOf<UAnimInstance> UnArmedAnimLayer;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Layers")
	TSubclassOf<UAnimInstance> PistolAnimLayer;

	UPROPERTY(EditDefaultsOnly, Category = "Animation | Layers")
	TSubclassOf<UAnimInstance> RifleAnimLayer;
};
