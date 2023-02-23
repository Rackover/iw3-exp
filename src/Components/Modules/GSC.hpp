#pragma once

namespace Components
{
	class GSC : public Component
	{
	public:
		const char* GetName() override { return "GSC"; };

		static void UpgradeGSC(const std::string& filePath, std::function<void(std::string&)> f);
		static void ConvertMainGSC(std::string& contents);
		static void ConvertFXGSC(std::string& contents);
		static void ConvertToStrictCreateFX(std::string& data);
		static void PatchSpecularScale(std::string& data);
		static void ConvertMainFXGSC(std::string& contents);
		static void ConvertMainArtGSC(std::string& contents);
		static std::string GetAdditionalModelsListPath()
		{
			return Utils::VA("%s/additionalModels.txt", AssetHandler::GetExportPath().data());
		}

	private:
		static void PatchReference(std::string& data, std::string _old, std::string _new);
		static void RemoveTeamDeclarations(std::string& data);
		static void DumpSounds(const std::string& data);
		static void DumpSubScripts(const std::string& data);
		static void DumpModels(const std::string& data);
		static void UpgradeCreateFog(std::string& contents);

		static std::unordered_set<std::string> dumpedSubscripts;
	};
}
