// Copyright BanMing

#include "Player/LyraALSPlayerController.h"

#include "Character/LyraALSCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Enums/EGate.h"

void ALyraALSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	LyraALSCharacterBase = InPawn ? Cast<ALyraALSCharacterBase>(InPawn) : NULL;
	if (LyraALSCharacterBase)
	{
		CharacterMovementComp = LyraALSCharacterBase->GetCharacterMovement();
	}
}

void ALyraALSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ALyraALSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &ThisClass::OnSwitchWeaponInput);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::OnLookInput);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveInput);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ThisClass::OnAimStartInput);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::OnAimEndInput);
}

void ALyraALSPlayerController::OnSwitchWeaponInput(const FInputActionValue& InputActionValue)
{
	FVector Value = InputActionValue.Get<FVector>();
	LyraALSCharacterBase->SwitchGun(static_cast<EGuns>(Value.X));
}

void ALyraALSPlayerController::OnLookInput(const FInputActionValue& InputActionValue)
{
	FVector2D Value = InputActionValue.Get<FVector2D>();
	AddYawInput(Value.X);
	AddPitchInput(Value.Y);
}

void ALyraALSPlayerController::OnMoveInput(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ALyraALSPlayerController::OnAimStartInput(const FInputActionValue& InputActionValue)
{
	LyraALSCharacterBase->SwitchGate(EGate::Walking);
}

void ALyraALSPlayerController::OnAimEndInput(const FInputActionValue& InputActionValue)
{
	LyraALSCharacterBase->SwitchGate(EGate::Jogging);
}
