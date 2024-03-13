// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPP_UGH : ModuleRules
{
	public CPP_UGH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
