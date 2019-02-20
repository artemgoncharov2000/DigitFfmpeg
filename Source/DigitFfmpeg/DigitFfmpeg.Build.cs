// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class DigitFfmpeg : ModuleRules
{
    private string FfmpegPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "ThirdParty", "ffmpeg")); }
    }
	public DigitFfmpeg(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);

        string LibrariesPath = Path.Combine(FfmpegPath, "lib");
        string IncludePath = Path.Combine(FfmpegPath, "include");
        string DllPath = Path.Combine(FfmpegPath, "dll");

        PrivateIncludePaths.Add(IncludePath);

        string[] dll = new string[]
        {
            "avcodec-58.dll",
            "avdevice-58.dll",
            "avfilter-7.dll",
            "avformat-58.dll",
            "avutil-56.dll",
            "postproc-55.dll",
            "swresample-3.dll",
            "swscale-5.dll"
        };

        foreach (var VARIABLE in dll)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(DllPath, VARIABLE));
        }

        string[] lib = new string[]
        {
            "avcodec.lib",
            "avdevice.lib",
            "avfilter.lib",
            "avformat.lib",
            "avutil.lib",
            "postproc.lib",
            "swresample.lib",
            "swscale.lib"
        };

        foreach (string VARIABLE in lib)
        {
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, VARIABLE));
        }



        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
