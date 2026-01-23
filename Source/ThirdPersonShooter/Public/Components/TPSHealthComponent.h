// Third Person Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THIRDPERSONSHOOTER_API UTPSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTPSHealthComponent();

	float GetHealth() const { return Health; }
	
	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return FMath::IsNearlyZero(Health); }
	
	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta=(ClampMin="0.0", ClampMax="100.0"))
	float MaxHealth = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool bAutoHeal;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealUpdateTime = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealDelay = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "bAutoHeal"))
	float HealModifier = 5.0f;


private:
	float Health = 0.0;
	FTimerHandle HealTimerHandle;
	
	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                           AController* InstigatedBy, AActor* DamageCauser);
	
	void HealUpdate();
	
	void SetHealth(float NewHealth);
};
