// Third Person Shooter Game. All rights reserved


#include "TPS_GameModeBase.h"

#include "Player/TPSBaseCharacter.h"
#include "Player/TPSPlayerController.h"
#include "UI/TPSGameHUD.h"

ATPS_GameModeBase::ATPS_GameModeBase()
{
	DefaultPawnClass = ATPSBaseCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	HUDClass = ATPSGameHUD::StaticClass();
}
