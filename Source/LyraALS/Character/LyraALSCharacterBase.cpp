// Copyright BanMing

#include "Character/LyraALSCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/AnimationInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/GateSetting.h"

ALyraALSCharacterBase::ALyraALSCharacterBase()
{
	EquippedGun = EGuns::UnArmed;

	Pistol = CreateDefaultSubobject<USkeletalMeshComponent>("Pistol");
	Pistol->SetupAttachment(GetMesh(), SkeletonSockets.PistolUnEquipped);

	Rifle = CreateDefaultSubobject<USkeletalMeshComponent>("Rifle");
	Rifle->SetupAttachment(GetMesh(), SkeletonSockets.RifleUnEquipped);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	CameraAimingTimeline = CreateDefaultSubobject<UTimelineComponent>("CameraAimingTimeline");
}

void ALyraALSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AnimationInterface.SetObject(GetMesh()->GetAnimInstance());
	AnimationInterface.SetInterface(Cast<IAnimationInterface>(GetMesh()->GetAnimInstance()));

	SwitchGun(EGuns::UnArmed);
	SwitchGate(EGate::Jogging);

	CameraAimingTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

	FOnTimelineFloatStatic LerpAimingCameraFunc;
	LerpAimingCameraFunc.BindLambda([this](float Alpha) { SpringArm->TargetArmLength = FMath::Lerp(400.f, 200.f, Alpha); });
	CameraAimingTimeline->AddInterpFloat(AimingTimelineCurve, LerpAimingCameraFunc);
}

void ALyraALSCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateGroundDistance();
}

void ALyraALSCharacterBase::SwitchGun(EGuns Gun)
{
	EquippedGun = Gun;
	AnimationInterface->ReceiveEquippedGun(Gun);

	switch (Gun)
	{
		case EGuns::InvalidValue:
		case EGuns::UnArmed:
			GetMesh()->LinkAnimClassLayers(UnArmedAnimLayer);
			break;
		case EGuns::Pistol:
			GetMesh()->LinkAnimClassLayers(PistolAnimLayer);
			break;
		case EGuns::Rifle:
			GetMesh()->LinkAnimClassLayers(RifleAnimLayer);
			break;
	}
	ChangeWeapon();
}

void ALyraALSCharacterBase::SwitchGate(EGate Gate)
{
	if (CurrentGate == EGate::Crouching && Gate != EGate::Crouching)
	{
		UnCrouch();
	}
	if (CurrentGate != EGate::Crouching && Gate == EGate::Crouching)
	{
		Crouch();
	}

	CurrentGate = Gate;
	AnimationInterface->ReceiveCurrentGate(CurrentGate);
	FGateSetting* GateSetting = GateSettingDataTable->FindRow<FGateSetting>(GetCurGateName(), "");

	UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();
	if (CharacterMovementComp && GateSetting)
	{
		CharacterMovementComp->MaxAcceleration = GateSetting->MaxAcceleration;
		CharacterMovementComp->BrakingDecelerationWalking = GateSetting->BrakingDeceleration;
		CharacterMovementComp->BrakingFrictionFactor = GateSetting->BrakingFrictionFactor;
		CharacterMovementComp->BrakingFriction = GateSetting->BrakingFriction;
		CharacterMovementComp->bUseSeparateBrakingFriction = GateSetting->bUseSeparateBrakingFriction;
		if (CurrentGate == EGate::Crouching)
		{
			CharacterMovementComp->MaxWalkSpeedCrouched = GateSetting->MaxWalkSpeed;
		}
		else
		{
			CharacterMovementComp->MaxWalkSpeed = GateSetting->MaxWalkSpeed;
		}
	}
}

void ALyraALSCharacterBase::OpenWeaponFire()
{
	const bool bIsEquipped = GetEquippedGun() == EGuns::Rifle || GetEquippedGun() == EGuns::Pistol;
	const bool bIsRightGate = GetCurrentGate() == EGate::Crouching || GetCurrentGate() == EGate::Walking;
	if (bIsEquipped && bIsRightGate && bCanFire && WeaponInfo.Contains(EquippedGun))
	{
		bCanFire = false;
		FWeaponInfo& Info = WeaponInfo[EquippedGun];
		PlayAnimMontage(Info.CharacterFireAnim);
		GetCurWeapon()->PlayAnimation(Info.WeaponFireAnim, false);

		GetWorldTimerManager().SetTimer(
			FireTimerHandle,
			[this, Info]()
			{
				bCanFire = true;
				GetWorldTimerManager().ClearTimer(FireTimerHandle);
				FHitResult OutHit;

				if (FireLineTrace(OutHit))
				{
					UGameplayStatics::PlaySoundAtLocation(this, Info.ImpactPlasterSound, OutHit.ImpactPoint);
					UGameplayStatics::PlaySoundAtLocation(this, Info.ImpactPlasterDebrisSound, OutHit.ImpactPoint);
				}
			},
			Info.FireRate, false);
	}
}

void ALyraALSCharacterBase::Reload()
{
	if (WeaponInfo.Contains(EquippedGun))
	{
		FWeaponInfo& Info = WeaponInfo[EquippedGun];

		PlayAnimMontage(Info.CharacterReloadAnim);
		GetCurWeapon()->PlayAnimation(Info.WeaponReloadAnim, false);
	}
}

void ALyraALSCharacterBase::Aiming(bool bStart)
{
	EGate TargetGate = bStart ? EGate::Walking : EGate::Jogging;
	if (CurrentGate == EGate::Crouching)
	{
		TargetGate = EGate::Crouching;
	}
	SwitchGate(TargetGate);

	if (CameraAimingTimeline->IsPlaying())
	{
		CameraAimingTimeline->Stop();
	}
	if (bStart)
	{
		CameraAimingTimeline->PlayFromStart();
	}
	else
	{
		CameraAimingTimeline->ReverseFromEnd();
	}
}

void ALyraALSCharacterBase::ChangeWeapon()
{
	Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SkeletonSockets.PistolUnEquipped);
	Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SkeletonSockets.RifleUnEquipped);
	switch (EquippedGun)
	{
		case EGuns::Pistol:
			Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SkeletonSockets.WeaponEquipped);
			break;
		case EGuns::Rifle:
			Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SkeletonSockets.WeaponEquipped);
			break;
	}
	bCanFire = true;
}

void ALyraALSCharacterBase::UpdateGroundDistance()
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		FHitResult OutHit;

		FVector Start = GetActorLocation() - FVector(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector End = GetActorLocation() - FVector(0.f, 0.f, 1000.f);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility) && AnimationInterface)
		{
			AnimationInterface->ReceiveGroundDistance(OutHit.Distance);
		}
	}
}

USkeletalMeshComponent* ALyraALSCharacterBase::GetCurWeapon()
{
	switch (EquippedGun)
	{
		case EGuns::Pistol:
			return Pistol;
			break;
		case EGuns::Rifle:
			return Rifle;
			break;
	}
	return nullptr;
}

FName ALyraALSCharacterBase::GetCurGateName()
{
	FString Res;

	switch (EquippedGun)
	{
		case EGuns::UnArmed:
			Res.Append("UnArmed");
			break;
		case EGuns::Pistol:
			Res.Append("Pistol");
			break;
		case EGuns::Rifle:
			Res.Append("Rifle");
			break;
	}
	switch (CurrentGate)
	{
		case EGate::Walking:
			Res.Append("Walk");
			break;
		case EGate::Jogging:
			Res.Append("Jog");
			break;
		case EGate::Crouching:
			Res.Append("Crouch");
			break;
	}
	return FName(Res);
}

bool ALyraALSCharacterBase::FireLineTrace(FHitResult& OutHit)
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Camera->GetForwardVector() * 500000.f;
	return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility);
}