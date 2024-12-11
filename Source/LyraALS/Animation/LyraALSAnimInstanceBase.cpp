// Copyright BanMing

#include "Animation/LyraALSAnimInstanceBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetAnimationLibrary.h"

void ULyraALSAnimInstanceBase::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	if (GetOwningActor())
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = GetOwningActor()->GetComponentByClass<UCharacterMovementComponent>())
		{
			CharacterMovementComp = CharacterMovementComponent;
		}
	}
}

void ULyraALSAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
#if !UE_BUILD_SHIPPING
	Debug();
#endif	  // UE_BUILD_SHIPPING
}
void ULyraALSAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (CharacterMovementComp)
	{
		CharacterVelocity = CharacterMovementComp->Velocity;
		CharacterVelocity2D.X = CharacterVelocity.X;
		CharacterVelocity2D.Y = CharacterVelocity.Y;
		CharacterVelocity2D.Z = 0.f;
	}

	if (GetOwningActor())
	{
		WorldRotation = GetOwningActor()->GetActorRotation();
		VelocityLocomotionAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity2D, WorldRotation);

		LocomotionDirection = CalculateLocomotionDirection(VelocityLocomotionAngle, -130.f, 130.f, -50.f, 50.f, LocomotionDirection, 20.f);
	}
}

void ULyraALSAnimInstanceBase::ReceiveEquippedGun(EGuns InEquippedGun)
{
	EquippedGun = InEquippedGun;
}

void ULyraALSAnimInstanceBase::ReceiveCurrentGate(EGate InGate)
{
	CurrentGate = InGate;
}

ELocomotionDirection ULyraALSAnimInstanceBase::CalculateLocomotionDirection(
	float CurVelocityLocomotionAngle, float BackwardMin, float BackwardMax, float ForwardMin, float ForwardMax, ELocomotionDirection CurrentDirection, float DeadZone)
{
	// // // // // // // // // // // // // // //
	// ForwardMin	 Forward	BackwardMax
	//			 ↖		0°		↗
	//      	  - - - - - - -
	//      	  | ↖ | ↑ | ↗ |
	//      	  - - - - - - -
	//  -90° Left | ← | o | → | Right 90°
	//      	  - - - - - - -
	//      	  | ↙ | ↓ | ↘ |
	//      	  - - - - - - -
	//			↙	-180°/180° ↘
	// BackwardMin	 Backward	BackwardMax
	// // // // // // // // // // // // // // //
	
	// Check dead zone
	switch (CurrentDirection)
	{
		case ELocomotionDirection::Forward:
			if (CurVelocityLocomotionAngle > ForwardMin - DeadZone && CurVelocityLocomotionAngle < ForwardMax + DeadZone)
			{
				return ELocomotionDirection::Forward;
			}
			break;
		case ELocomotionDirection::Backward:
			if (CurVelocityLocomotionAngle > BackwardMax - DeadZone || CurVelocityLocomotionAngle < BackwardMin + DeadZone)
			{
				return ELocomotionDirection::Backward;
			}
			break;
		case ELocomotionDirection::Left:
			if (CurVelocityLocomotionAngle > BackwardMin - DeadZone && CurVelocityLocomotionAngle < ForwardMin + DeadZone)
			{
				return ELocomotionDirection::Left;
			}
			break;
		case ELocomotionDirection::Right:
			if (CurVelocityLocomotionAngle > ForwardMax - DeadZone && CurVelocityLocomotionAngle < BackwardMax + DeadZone)
			{
				return ELocomotionDirection::Right;
			}
			break;
	}

	if (CurVelocityLocomotionAngle > BackwardMax || CurVelocityLocomotionAngle < BackwardMin)
	{
		return ELocomotionDirection::Backward;
	}
	else if (CurVelocityLocomotionAngle > ForwardMin && CurVelocityLocomotionAngle < ForwardMax)
	{
		return ELocomotionDirection::Forward;
	}
	else if (CurVelocityLocomotionAngle > 0.f)
	{
		return ELocomotionDirection::Right;
	}

	return ELocomotionDirection::Left;
}

#if !UE_BUILD_SHIPPING
void ULyraALSAnimInstanceBase::Debug()
{
	if (DebugOptions.bShowGateData)
	{
		DebugPrintString("Current Gate", StaticEnum<EGate>()->GetNameStringByValue((int32) CurrentGate), 1231, FColor::Blue);
	}

	if (DebugOptions.bShowLocomotionData)
	{
		DebugDrawVector("Velocity", CharacterVelocity, FColor::Green);
		DebugPrintFloat("Velocity", CharacterVelocity2D.Length(), 1232, FColor::Green);
		DebugPrintFloat("Velocity Locomotion Angle", VelocityLocomotionAngle, 1233, FColor::Yellow);
		DebugPrintString("Locomotion Direction", StaticEnum<ELocomotionDirection>()->GetNameStringByValue((int32) LocomotionDirection), 1234, FColor::Blue);
	}
}

void ULyraALSAnimInstanceBase::DebugPrintFloat(FString Name, float Value, int32 Key, FColor DisplayColor)
{
	GEngine->AddOnScreenDebugMessage(Key, 2, DisplayColor, FString::Format(TEXT("{0} = {1}"), {Name, Value}));
}

void ULyraALSAnimInstanceBase::DebugPrintString(FString Name, FString Value, int32 Key, FColor DisplayColor)
{
	GEngine->AddOnScreenDebugMessage(Key, 2, DisplayColor, FString::Format(TEXT("{0} = {1}"), {Name, Value}));
}

void ULyraALSAnimInstanceBase::DebugDrawVector(FString Name, FVector Value, FColor DisplayColor)
{
	FVector Start = GetOwningActor()->GetActorLocation() * FVector(1.f, 1.f, 0);
	FVector Target = Start + Value.GetClampedToMaxSize(100.f) * FVector(1.f, 1.f, 0);
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), Start, Target, 5.0, DisplayColor, 0, 3);
	UKismetSystemLibrary::DrawDebugString(GetWorld(), Target, Name, nullptr, DisplayColor);
}
#endif	  // UE_BUILD_SHIPPING