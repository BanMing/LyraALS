// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Enums/EGuns.h"
#include "GameFramework/Character.h"

#include "CharacterBase.generated.h"

UCLASS()
class LYRAALS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	EGuns EquippedGun;
};
