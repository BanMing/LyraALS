// Copyright BanMing

#include "Character/LyraALSCharacterBase.h"

#include "Interface/AnimationInterface.h"
ALyraALSCharacterBase::ALyraALSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALyraALSCharacterBase::SwitchGun(EGuns Gun)
{
	EquipedGun = Gun;
	AnimationInterface->RecieveEquipedGun(Gun);
}

void ALyraALSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AnimationInterface.SetObject(GetMesh()->GetAnimInstance());
	AnimationInterface.SetInterface(Cast<IAnimationInterface>(GetMesh()->GetAnimInstance()));
}
