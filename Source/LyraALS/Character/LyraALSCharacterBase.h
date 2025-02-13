// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Enums/EGate.h"
#include "Enums/EGuns.h"
#include "GameFramework/Character.h"
#include "Struct/SkeletonSockets.h"
#include "Struct/WeaponInfo.h"

#include "LyraALSCharacterBase.generated.h"

class IAnimationInterface;
class UAnimInstance;
class USpringArmComponent;
class UCameraComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()

class LYRAALS_API ALyraALSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALyraALSCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds);

public:
	FORCEINLINE EGate GetCurrentGate() const
	{
		return CurrentGate;
	}

	FORCEINLINE EGuns GetEquippedGun() const
	{
		return EquippedGun;
	}

public:
	void SwitchGun(EGuns Gun);
	void SwitchGate(EGate Gate);
	void OpenWeaponFire();
	void Reload();
	void Aiming(bool bStart);

protected:
	void ChangeWeapon();
	void UpdateGroundDistance();
	USkeletalMeshComponent* GetCurWeapon();
	FName GetCurGateName();
	bool FireLineTrace(FHitResult& OutHit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<USkeletalMeshComponent> Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<USkeletalMeshComponent> Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UTimelineComponent> CameraAimingTimeline;

protected:
	UPROPERTY(BlueprintReadOnly)
	EGuns EquippedGun = EGuns::UnArmed;

	UPROPERTY(BlueprintReadOnly)
	EGate CurrentGate = EGate::Walking;

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IAnimationInterface> AnimationInterface;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Layers")
	TSubclassOf<UAnimInstance> UnArmedAnimLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Layers")
	TSubclassOf<UAnimInstance> PistolAnimLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Layers")
	TSubclassOf<UAnimInstance> RifleAnimLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation | Layers")
	FSkeletonSockets SkeletonSockets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCurveFloat> AimingTimelineCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EGuns, FWeaponInfo> WeaponInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gate Setting")
	TObjectPtr<UDataTable> GateSettingDataTable;

	UPROPERTY(BlueprintReadOnly)
	bool bCanFire = false;

	FTimerHandle FireTimerHandle;
};
