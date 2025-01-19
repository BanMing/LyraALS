// Copyright BanMing

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Enums/ELocomotionDirection.h"
#include "Enums/ERootYawOffsetMode.h"
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
	void ReceiveGroundDistance(float InGroundDistance) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Locomotion Data", meta = (BlueprintThreadSafe))
	ELocomotionDirection CalculateLocomotionDirection(float CurVelocityLocomotionAngle, float BackwardMin, float BackwardMax, float ForwardMin, float ForwardMax, ELocomotionDirection CurrentDirection, float DeadZone);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void OnIdleStateUpdate(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	void SetupJumpApexPose(const FAnimUpdateContext& UpdateContext, const FAnimNodeReference& Node);
#pragma region Update Data
protected:
	void UpdateGate();
	void UpdateJumpInfo(float DeltaSeconds);
	void UpdateLocationData();
	void UpdateVelocityData();
	void UpdateAccelerationData();
	void UpdateRotationData(float DeltaSeconds);
	void UpdateLocomotionData();
	void ProcessTurnYawCurve();
#pragma endregion

#pragma region Debug
#if !UE_BUILD_SHIPPING
protected:
	void Debug();
	void DebugPrintFloat(FString Name, float Value, int32 Key, FColor DisplayColor);
	void DebugPrintString(FString Name, FString Value, int32 Key, FColor DisplayColor);
	void DebugDrawVector(FString Name, FVector Value, FColor DisplayColor);
#endif	  // UE_BUILD_SHIPPING
#pragma endregion

public:
	FORCEINLINE EGate GetCurrentGate() const
	{
		return CurrentGate;
	}

	FORCEINLINE ELocomotionDirection GetLocomotionDirection() const
	{
		return LocomotionDirection;
	}

	FORCEINLINE ELocomotionDirection GetAccelerationLocomotionDirection() const
	{
		return AccelerationLocomotionDirection;
	}

	FORCEINLINE FVector GetCharacterVelocity2D() const
	{
		return CharacterVelocity2D;
	}

	FORCEINLINE FVector GetCurAcceleration2D() const
	{
		return CurAcceleration2D;
	}

	FORCEINLINE FVector GetCurAccelerationData() const
	{
		return CurAcceleration;
	}

	FORCEINLINE UCharacterMovementComponent* GetCharacterMovementComp() const
	{
		return CharacterMovementComp;
	}

	FORCEINLINE float GetDeltaLocation() const
	{
		return DeltaLocation;
	}

	FORCEINLINE void SetPivotAcceleration2D(FVector InPivotAcceleration2D)
	{
		PivotAcceleration2D = InPivotAcceleration2D;
	}

	FORCEINLINE float GetRootYawOffset() const
	{
		return RootYawOffset;
	}

	FORCEINLINE bool GetIsCrouching() const
	{
		return bIsCrouching;
	}

	FORCEINLINE bool GetIsLastFrameCrouching() const
	{
		return bIsLastFrameCrouching;
	}

	FORCEINLINE bool GetIsCrouchStateChanged() const
	{
		return bIsCrouchStateChanged;
	}

	FORCEINLINE float GetIncomingGroundDistance() const
	{
		return IncomingGroundDistance;
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Rotation Data")
	float LeanFactor = 5.f;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Guns")
	EGuns EquippedGun;

	UPROPERTY(BlueprintReadOnly, Category = "Gate")
	EGate CurrentGate;

	UPROPERTY(BlueprintReadOnly, Category = "Gate")
	EGate IncomingGate;

	UPROPERTY(BlueprintReadOnly, Category = "Gate")
	EGate LastFrameGate;

	UPROPERTY(BlueprintReadOnly, Category = "Gate")
	bool bIsGateChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Location Data")
	FVector WorldLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Location Data")
	FVector LastFrameWorldLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Location Data")
	float DeltaLocation;

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

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float RootYawOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	ERootYawOffsetMode RootYawOffsetMode;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float TurnYawCurveValue;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float LastFrameTurnYawCurveValue;

	UPROPERTY(BlueprintReadOnly, Category = "Rotation Data")
	float AimPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	float VelocityLocomotionAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	float VelocityLocomotionAngleWithOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	float AccelerationLocomotionAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	ELocomotionDirection AccelerationLocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	ELocomotionDirection LocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion Data")
	ELocomotionDirection LastFrameLocomotionDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data")
	FVector CurAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data")
	FVector CurAcceleration2D;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data")
	FVector PivotAcceleration2D;

	UPROPERTY(BlueprintReadOnly, Category = "Acceleration Data")
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "Crouch Data")
	bool bIsCrouching;

	UPROPERTY(BlueprintReadOnly, Category = "Crouch Data")
	bool bIsLastFrameCrouching;

	UPROPERTY(BlueprintReadOnly, Category = "Crouch Data")
	bool bIsCrouchStateChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bIsJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float TimeToJumpApex;

	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float IncomingGroundDistance;

	UPROPERTY(BlueprintReadOnly, Category = "Jump Data")
	float TimeFalling;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FAnimInstDebugOptions DebugOptions;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComp;
};
