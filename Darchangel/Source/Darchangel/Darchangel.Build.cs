// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Darchangel : ModuleRules
{
	public Darchangel(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {"Core",														
															"CoreUObject",
															"Engine",
															"InputCore",
															"CableComponent",
															"HeadMountedDisplay",
															"AIModule",
															"NavigationSystem",
															"GameplayTasks",
															"UMG",
															"Slate",
															"SlateCore"															
															});

		PrivateDependencyModuleNames.AddRange(new string[] { "CableComponent" });
	}
}
