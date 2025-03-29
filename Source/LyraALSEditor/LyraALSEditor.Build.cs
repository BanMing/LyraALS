// Copyright Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class LyraALSEditor : ModuleRules
    {
        public LyraALSEditor(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicIncludePaths.AddRange(
                new string[] {
                    "LyraALSEditor"
                }
            );

            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "AnimationCore",
                "AnimationWarpingRuntime",
                "AnimGraph",
                "AnimGraphRuntime",
                "AnimationModifiers",
                "AnimationBlueprintLibrary",
                "AnimationModifierLibrary",
                "Core",
                "CoreUObject",
                "Engine",
                "BlueprintGraph",
                        "EditorFramework",
                        "Kismet",
                        "UnrealEd",
                "LyraALS",
            });

            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "SlateCore",
            });

        }
    }
}
