// Third Person Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSBaseWeapon.generated.h"

UCLASS()
class THIRDPERSONSHOOTER_API ATPSBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ATPSBaseWeapon();

	virtual void Fire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleFlashSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float DamageAmount = 10.0f;

	virtual void BeginPlay() override;

	void MakeDamage(const FHitResult& HitResult);
	void MakeShot();
};
