// Third Person Shooter Game. All rights reserved


#include "Weapon/TPSBaseWeapon.h"

#include "Components/TPSHealthComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Player/TPSBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ATPSBaseWeapon::ATPSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMeshComponent");
	RootComponent = WeaponMeshComponent;
}

void ATPSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMeshComponent);
}

void ATPSBaseWeapon::Fire()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("WEAPON FIRED"));
	MakeShot();
}

void ATPSBaseWeapon::MakeShot()
{
	if (!GetWorld()) return;

	const ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	const APlayerController* Controller = Player->GetController<APlayerController>();

	if (!Controller) return;

	FVector ViewLocation;
	FRotator ViewRotation;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);


	const FTransform SocketTransform = WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);

	const FVector TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);

	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Yellow, false, 3.0f, 0,
		              3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

		UE_LOG(LogBaseWeapon, Display, TEXT("Bone name: %s"), *HitResult.BoneName.ToString());
	}
	else
	{
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Yellow, false, 3.0f, 0, 3.0f);
	}
}

void ATPSBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;
	
	const ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	AController* Controller = Player->GetController<APlayerController>();

	if (!Controller) return;
	
	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), Controller, this);
}
