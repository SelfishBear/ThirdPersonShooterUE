// Third Person Shooter Game. All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class ThirdPersonShooterTarget : TargetRules
{
	public ThirdPersonShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "ThirdPersonShooter" } );
	}
}
