using UnrealBuildTool;
using System.IO;


public class Otoshiyori : ModuleRules
{
	public Otoshiyori(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Http", "ImageWrapper", "Paper2D", "Json", "JsonUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { /* "Settings" */ });

        PrivateIncludePaths.Add("Otoshiyori/Private");

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            PrivateIncludePaths.Add("Otoshiyori/Private/Android");

            string UPLPath = Utils.MakePathRelativeTo(ModuleDirectory, BuildConfiguration.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(UPLPath, "LocationService_UPL.xml")));
        }
    }
}
