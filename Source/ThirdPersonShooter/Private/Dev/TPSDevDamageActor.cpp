// Third Person Shooter Game. All rights reserved


#include "Dev/TPSDevDamageActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ATPSDevDamageActor::ATPSDevDamageActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	
	
}

void ATPSDevDamageActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSDevDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, bDoFullDamage);
}
