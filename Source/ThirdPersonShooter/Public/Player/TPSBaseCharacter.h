// Third Person Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSBaseCharacter.generated.h"

class UTPSWeaponComponent;
class ATPSBaseWeapon;
class UTextRenderComponent;
class UTPSHealthComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class THIRDPERSONSHOOTER_API ATPSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATPSBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	UTPSHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	UTextRenderComponent* HealthTextComponent;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Components")
	UTPSWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category="Movement")
	float GetMovementDirection() const;

private:
	bool WantToRun = false;
	bool IsMovingForward = false;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void OnStartRunning();
	void OnStopRunning();

	void OnDeath();
	void OnHealthChanged(float NewHealth);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
};
