// Copyright BanMing

#include "Player/LyraALSPlayerController.h"

#include "Character/LyraALSCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ALyraALSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	LyraALSCharacterBase = InPawn ? Cast<ALyraALSCharacterBase>(InPawn) : NULL;
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
