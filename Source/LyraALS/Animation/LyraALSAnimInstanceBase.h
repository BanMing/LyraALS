// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Interface/AnimationInterface.h"
#include "Struct/AnimInstDebugOptions.h"

#include "LyraALSAnimInstanceBase.generated.h"

class UCharacterMovementComponent;

/**
 *
 */
UCLASS()
class LYRAALS_API ULyraALSAnimInstanceBase : public UAnimInstance, public IAnimationInterface
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

public:
	// Inherited via IAnimationInterface
	virtual void ReceiveEquippedGun(EGuns InEquipedGun) override;
	virtual void ReceiveCurrentGate(EGate InGate) override;

#if !UE_BUILD_SHIPPING
protected:
	void Debug();
	void DebugPrintFloat(FString Name, float Value, int32 Key, FColor DisplayColor);
	void DebugPrintString(FString Name, FString Value, int32 Key, FColor DisplayColor);
	void DebugDrawVector(FString Name, FVector Value, FColor DisplayColor);
#endif	  // UE_BUILD_SHIPPING

protected:
	UPROPERTY(BlueprintReadOnly)
	EGuns EquippedGun;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EGate CurrentGate;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector CharacterVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")

	FVector2D CharacterVelocity2D;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FAnimInstDebugOptions DebugOptions;

protected:
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComp;
};
