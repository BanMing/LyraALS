// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "LyraALSPlayerController.generated.h"
class ALyraALSCharacterBase;
class UInputMappingContext;
class UInputAction;
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

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SwitchWeaponAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	TObjectPtr<ALyraALSCharacterBase> LyraALSCharacterBase;
};
