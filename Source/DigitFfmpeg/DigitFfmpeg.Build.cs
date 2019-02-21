// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class DigitFfmpeg : ModuleRules
{
    private string FfmpegPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "ThirdParty", "ffmpeg")); }
    }

    private string UProjectPath
    {
        get { return Directory.GetParent(ModuleDirectory).Parent.FullName; }
    }

    private void CopyToBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string binariesDir = Path.Combine(UProjectPath, "Binaries", Target.Platform.ToString());
        string filename = Path.GetFileName(Filepath);

        //System.Console.WriteLine("Writing file " + Filepath + " to " + binariesDir);

        if (!Directory.Exists(binariesDir))
            Directory.CreateDirectory(binariesDir);

        if (!File.Exists(Path.Combine(binariesDir, filename)))
            File.Copy(Filepath, Path.Combine(binariesDir, filename), true);
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

        string Platform = Target.Platform == UnrealTargetPlatform.Win64 ? "x64" : "win32";


        string LibrariesPath = Path.Combine(FfmpegPath, "lib", Platform);
        string IncludePath = Path.Combine(FfmpegPath, "include");
        string DllPath = Path.Combine(FfmpegPath, "bin", Platform);

        PublicLibraryPaths.Add(LibrariesPath);

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
            PublicAdditionalLibraries.Add(VARIABLE);

        }

        string[] dlls = new string[]
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


        foreach (string VARIABLE in dlls)
        {

            PublicDelayLoadDLLs.Add(VARIABLE);
            CopyToBinaries(Path.Combine(DllPath, VARIABLE), Target);
            RuntimeDependencies.Add(new RuntimeDependency(Path.Combine(DllPath, VARIABLE)));
        }

        PrivateIncludePaths.Add(IncludePath);

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
