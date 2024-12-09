// Copyright BanMing

#include "Character/LyraALSCharacterBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/AnimationInterface.h"

ALyraALSCharacterBase::ALyraALSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	EquippedGun = EGuns::UnArmed;
}

void ALyraALSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	AnimationInterface.SetObject(GetMesh()->GetAnimInstance());
	AnimationInterface.SetInterface(Cast<IAnimationInterface>(GetMesh()->GetAnimInstance()));

	SwitchGun(EGuns::UnArmed);
	SwitchGate(EGate::Jogging);
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
}

void ALyraALSCharacterBase::SwitchGate(EGate Gate)
{
	CurrentGate = Gate;
	AnimationInterface->ReceiveCurrentGate(CurrentGate);

	UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();
	if (CharacterMovementComp && GateSettingMap.Contains(CurrentGate))
	{
		CharacterMovementComp->MaxWalkSpeed = GateSettingMap[CurrentGate].MaxWalkSpeed;
		CharacterMovementComp->MaxAcceleration = GateSettingMap[CurrentGate].MaxAcceleration;
		CharacterMovementComp->BrakingDecelerationWalking = GateSettingMap[CurrentGate].BrakingDeceleration;
		CharacterMovementComp->BrakingFrictionFactor = GateSettingMap[CurrentGate].BrakingFrictionFactor;
		CharacterMovementComp->BrakingFriction = GateSettingMap[CurrentGate].BrakingFriction;
		CharacterMovementComp->bUseSeparateBrakingFriction = GateSettingMap[CurrentGate].bUseSeparateBrakingFriction;
	}
}
