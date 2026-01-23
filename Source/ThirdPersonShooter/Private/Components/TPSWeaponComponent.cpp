// Third Person Shooter Game. All rights reserved


#include "Components/TPSWeaponComponent.h"

#include "GameFramework/Character.h"
#include "Weapon/TPSBaseWeapon.h"

class ATPSBaseWeapon;

UTPSWeaponComponent::UTPSWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTPSWeaponComponent::Fire()
{
	if (!CurrentWeapon) return;
	
	CurrentWeapon->Fire();
}


void UTPSWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}


void UTPSWeaponComponent::SpawnWeapon()
{
	if (GetWorld())
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character) return;
		
		CurrentWeapon = GetWorld()->SpawnActor<ATPSBaseWeapon>(WeaponClass);
		if (!CurrentWeapon) return;
		
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
		CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
		CurrentWeapon->SetOwner(Character);
	}
}
