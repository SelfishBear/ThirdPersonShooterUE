// Third Person Shooter Game. All rights reserved


#include "Components/TPSCharacterMovementComponent.h"

#include "Player/TPSBaseCharacter.h"

float UTPSCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ATPSBaseCharacter* Player = Cast<ATPSBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
