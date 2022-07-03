#pragma once

namespace Components
{
	class GSC : public Component
	{
	public:
		const char* getName() override { return "GSC"; };

		static void UpgradeGSC(std::string filePath, std::function<void(std::string&)> f);
		static void ConvertMainGSC(std::string& contents);
		static void ConvertFXGSC(std::string& contents);
		static void ConvertMainFXGSC(std::string& contents);
		static void ConvertMainArtGSC(std::string& contents);

	private:
		static void PatchReference(std::string& data, std::string _old, std::string _new);
		static void RemoveTeamDeclarations(std::string& data);
		static void DumpSounds(std::string data);
		static void DumpSubScripts(std::string data);
		static void UpgradeCreateFog(std::string& contents);
	};
}
