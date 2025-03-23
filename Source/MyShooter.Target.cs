// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using static UnrealBuildTool.ModuleRules;

public class MyShooterTarget : TargetRules
{
	public MyShooterTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("MyShooter");
	}
}
