// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class myRTS : ModuleRules
{
	public myRTS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
            "NavigationSystem", "AIModule", "GameplayTasks", "GameplayAbilities", "GameplayTags","Slate", "SlateCore" });
    }
}
