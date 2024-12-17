// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Enums/ELocomotionDirection.h"
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

protected:
	UFUNCTION(BlueprintCallable, Category = "Locomotion Data", meta = (BlueprintThreadSafe))
	ELocomotionDirection CalculateLocomotionDirection(float CurVelocityLocomotionAngle, float BackwardMin, float BackwardMax, float ForwardMin, float ForwardMax, ELocomotionDirection CurrentDirection, float DeadZone);

#if !UE_BUILD_SHIPPING
protected:
	void Debug();
	void DebugPrintFloat(FString Name, float Value, int32 Key, FColor DisplayColor);
	void DebugPrintString(FString Name, FString Value, int32 Key, FColor DisplayColor);
	void DebugDrawVector(FString Name, FVector Value, FColor DisplayColor);
#endif	  // UE_BUILD_SHIPPING
public:
	FORCEINLINE EGate GetCurrentGate() const
	{
		return CurrentGate;
	}

	FORCEINLINE ELocomotionDirection GetLocomotionDirection() const
	{
		return LocomotionDirection;
	}

	FORCEINLINE FVector GetCharacterVelocity2D() const
	{
		return CharacterVelocity2D;
	}

	FORCEINLINE FVector GetCurAccelerationData() const
	{
		return CurAcceleration;
	}

	FORCEINLINE UCharacterMovementComponent* GetCharacterMovementComp() const
	{
		return CharacterMovementComp;
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Rotation Data")
	float LeanFactor = 5.f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Guns")
	EGuns EquippedGun;

	UPROPERTY(BlueprintReadOnly, Category = "Gate")
	EGate CurrentGate;

	UPROPERTY(BlueprintReadOnly, Category = "Velocity Data")
	FVector CharacterVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Velocity Data")
	FVector CharacterVelocity2D;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	FRotator WorldRotation;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float ActorYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float LastFrameActorYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float DeltaActorYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float LeanAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	float VelocityLocomotionAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	ELocomotionDirection LocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data")
	FVector CurAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data")
	FVector CurAcceleration2D;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data")
	bool bIsAccelerating;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FAnimInstDebugOptions DebugOptions;

protected:
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComp;
};
