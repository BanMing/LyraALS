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
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::OnCrouchInput);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::OnStartJumpInput);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::OnEndJumpInput);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ThisClass::OnFireInput);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::OnReloadInput);
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
	LyraALSCharacterBase->Aiming(true);
}

void ALyraALSPlayerController::OnAimEndInput(const FInputActionValue& InputActionValue)
{
	LyraALSCharacterBase->Aiming(false);
}

void ALyraALSPlayerController::OnCrouchInput(const FInputActionValue& InputActionValue)
{
	switch (LyraALSCharacterBase->GetCurrentGate())
	{
		case EGate::Walking:
		case EGate::Jogging:
			LyraALSCharacterBase->SwitchGate(EGate::Crouching);
			break;
		case EGate::Crouching:
			LyraALSCharacterBase->SwitchGate(EGate::Jogging);
			break;
	}
}

void ALyraALSPlayerController::OnStartJumpInput(const FInputActionValue& InputActionValue)
{
	LyraALSCharacterBase->Jump();
}

void ALyraALSPlayerController::OnEndJumpInput(const FInputActionValue& InputActionValue)
{
	LyraALSCharacterBase->StopJumping();
}

void ALyraALSPlayerController::OnFireInput(const FInputActionValue& InputActionValue)
{
	LyraALSCharacterBase->OpenWeaponFire();
}

void ALyraALSPlayerController::OnReloadInput(const FInputActionValue& InputActionValue)
{
	LyraALSCharacterBase->Reload();
}
