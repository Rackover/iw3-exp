#include "STDInclude.hpp"

namespace Components
{
	void GSC::DumpSounds(std::string data)
	{
		auto lines = Utils::Explode(data, '\n');
		for (auto& line : lines)
		{
			Utils::Replace(line, " ", "");

			std::string toMatch[2] = { 
				"ambientPlay\\(\"(.*)\"\\)"s ,
				"ent.v\\[\"soundalias\"\\]=\"(.*)\";"s
			};

			for (size_t i = 0; i < 2; i++)
			{
				std::regex regex(toMatch[i]);
				std::cmatch m;

				if (std::regex_search(line.data(), m, regex))
				{
					const auto& musicName = m[1];
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_SOUND, Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_SOUND, musicName.str().data()));
				}
			}
		}
	}

	void GSC::UpgradeGSC(std::string filePath, std::function<void(std::string&)> f)
	{
		if (Utils::FileExists(filePath))
		{
			std::string data = Utils::ReadFile(filePath);
			f(data);
			Utils::WriteFile(filePath, data);
		}
	}

	void GSC::ConvertMainGSC(std::string& data)
	{
		Utils::Replace(data, "\r\n", "\n");
		GSC::RemoveTeamDeclarations(data);
		GSC::DumpSounds(data);
	}

	void GSC::ConvertMainFXGSC(std::string& data)
	{
		Utils::Replace(data, "\r\n", "\n");
		GSC::PatchReference(data, "maps\\mp\\_fx", "common_scripts\\_fx");
		GSC::PatchReference(data, "maps\\mp\\_createfx", "common_scripts\\_createfx");
		GSC::PatchReference(data, "maps\\mp\\_utility", "common_scripts\\utility");
		GSC::DumpSounds(data);
	}

	void GSC::ConvertFXGSC(std::string& data)
	{
		Utils::Replace(data, "\r\n", "\n");
		GSC::PatchReference(data, "maps\\mp\\_createfx", "common_scripts\\_createfx");
		GSC::PatchReference(data, "maps\\mp\\_utility", "common_scripts\\utility");
		GSC::DumpSounds(data);
		data = Utils::VA("//_createfx generated. Do not touch!!\n%s", data.data());
	}

	void GSC::RemoveTeamDeclarations(std::string& data)
	{
		auto lines = Utils::Explode(data, '\n');
		std::string newData;
		for (auto& line : lines)
		{
			if (line.find("game"s) != std::string::npos)
			{
				if (line.find("game[\"attackers\"] ="s) == std::string::npos && line.find("game[\"defenders\"] ="s) == std::string::npos)
				{
					// We only allow attacker or defender declarations
					// The rest has to go - in iw4, it is deduced from the arena file and other things
					continue;
				}
			}

			newData.append(line);
			newData.append("\n");
		}

		data = newData;
	}

	void GSC::PatchReference(std::string& data, std::string _old, std::string _new)
	{
		// Remove absolute function paths
		Utils::Replace(data, Utils::VA("%s::", _old.data()), "");

		// Replace includes
		Utils::Replace(data, _old, _new);

		// Add our include in case it was not there
		data = Utils::VA("#include %s;\n%s", _new.data(), data.data());

		// Remove double includes (we might have created duplicates)
		auto lines = Utils::Explode(data, '\n');

		int count = 0;
		std::string newData;
		for (auto& line : lines)
		{
			if (line == Utils::VA("#include %s;", _new.data()) && count++ > 0)
			{
				continue;
			}

			if (Utils::StartsWith(line, "//_"))
			{
				continue;
			}

			newData.append(line);
			newData.append("\n");
		}

		data = newData;
	};
}
