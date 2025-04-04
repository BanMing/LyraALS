// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "LyraALSPlayerController.generated.h"

class ALyraALSCharacterBase;
class UInputMappingContext;
class UInputAction;
class UCharacterMovementComponent;
struct FInputActionValue;
/**
 *
 */
UCLASS()
class LYRAALS_API ALyraALSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	void OnSwitchWeaponInput(const FInputActionValue& InputActionValue);
	void OnLookInput(const FInputActionValue& InputActionValue);
	void OnMoveInput(const FInputActionValue& InputActionValue);
	void OnAimStartInput(const FInputActionValue& InputActionValue);
	void OnAimEndInput(const FInputActionValue& InputActionValue);
	void OnCrouchInput(const FInputActionValue& InputActionValue);
	void OnStartJumpInput(const FInputActionValue& InputActionValue);
	void OnEndJumpInput(const FInputActionValue& InputActionValue);
	void OnFireInput(const FInputActionValue& InputActionValue);
	void OnReloadInput(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SwitchWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;

private:
	TObjectPtr<ALyraALSCharacterBase> LyraALSCharacterBase;

	TObjectPtr<UCharacterMovementComponent> CharacterMovementComp;
};
