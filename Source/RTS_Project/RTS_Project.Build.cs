// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class RTS_Project : ModuleRules
{
	public RTS_Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "Slate",
            "SlateCore",
            "AIModule",
            "AnimGraphRuntime",
            "GameplayTasks",
            "RHI",
            "RenderCore",
            "NavigationSystem",
            "OnlineSubsystem",
            "OnlineSubsystemNull",
            "OnlineSubsystemSteam",
            "ProceduralMeshComponent",
            "Niagara"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

    }
}
