// Third Person Shooter Game. All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONSHOOTER_API ATPSGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;
	
private:
	void DrawCrosshair();
};
