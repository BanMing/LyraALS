// Copyright BanMing

#include "Character/LyraALSCharacterBase.h"

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

	GetMesh()->LinkAnimClassLayers(UnArmedAnimLayer);
}

void ALyraALSCharacterBase::SwitchGun(EGuns Gun)
{
	EquippedGun = Gun;
	AnimationInterface->RecieveEquipedGun(Gun);
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