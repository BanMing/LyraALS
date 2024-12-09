// Copyright BanMing

#include "Animation/LyraALSAnimInstanceBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetOwningActor()->GetActorLocation(), Target, 5.0, DisplayColor, 5, 3);
	UKismetSystemLibrary::DrawDebugString(GetWorld(), Target, Name);
}
#endif	  // UE_BUILD_SHIPPING