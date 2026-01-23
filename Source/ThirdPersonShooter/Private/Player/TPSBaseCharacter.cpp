// Third Person Shooter Game. All rights reserved


#include "Player/TPSBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/TPSCharacterMovementComponent.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon/TPSBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ATPSBaseCharacter::ATPSBaseCharacter(const FObjectInitializer& ObjInit) : Super(
	ObjInit.SetDefaultSubobjectClass<UTPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArm);

	HealthComponent = CreateDefaultSubobject<UTPSHealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);
	
	WeaponComponent = CreateDefaultSubobject<UTPSWeaponComponent>("WeaponComponent");
}

void ATPSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ATPSBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATPSBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ATPSBaseCharacter::OnGroundLanded);
}

void ATPSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATPSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);
	
	if (PlayerInputComponent)
	{
		PlayerInputComponent->BindAxis("MoveForward", this, &ATPSBaseCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ATPSBaseCharacter::MoveRight);
		PlayerInputComponent->BindAxis("LookUp", this, &ATPSBaseCharacter::AddControllerPitchInput);
		PlayerInputComponent->BindAxis("TurnAround", this, &ATPSBaseCharacter::AddControllerYawInput);
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSBaseCharacter::Jump);
		PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATPSBaseCharacter::OnStartRunning);
		PlayerInputComponent->BindAction("Run", IE_Released, this, &ATPSBaseCharacter::OnStopRunning);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UTPSWeaponComponent::Fire);
	}
}

bool ATPSBaseCharacter::IsRunning() const
{
	return WantToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ATPSBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ATPSBaseCharacter::MoveForward(float Value)
{
	IsMovingForward = Value > 0.1f;
	AddMovementInput(GetActorForwardVector(), Value);
}

void ATPSBaseCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ATPSBaseCharacter::OnStartRunning()
{
	WantToRun = true;
}

void ATPSBaseCharacter::OnStopRunning()
{
	WantToRun = false;
}

void ATPSBaseCharacter::OnDeath()
{
	UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead"), *GetName());
	
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(EName::Spectating);
	}
}

void ATPSBaseCharacter::OnHealthChanged(float NewHealth)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewHealth)));
}

void ATPSBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const double FallVelocityZ = -GetCharacterMovement()->Velocity.Z;

	UE_LOG(LogBaseCharacter, Display, TEXT("Landed with velocity Z = %.2f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;
	const float FinalDamage = FMath::GetMappedRangeValueClamped(
		LandedDamageVelocity, LandedDamage, -FallVelocityZ);

	UE_LOG(LogBaseCharacter, Display, TEXT("Final damage: %f"), FinalDamage);

	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}


