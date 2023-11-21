#include "STDInclude.hpp"

namespace Components
{
	std::unordered_set<std::string> GSC::dumpedSubscripts{};

	void GSC::DumpSounds(const std::string& data)
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
					AssetHandler::Dump(Game::IW3::XAssetType::ASSET_TYPE_SOUND, Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_SOUND, musicName.str().data()));
				}
			}
		}
	}

	void GSC::DumpSubScripts(const std::string& data)
	{
		std::vector<std::string> dumpBlacklist{
			Utils::VA("%s_fx", MapDumper::GetMapName().c_str()) // This one is special
		};

		auto lines = Utils::Explode(data, '\n');
		for (auto& line : lines)
		{
			Utils::Replace(line, " ", "");
			Utils::Replace(line, "\t", "");

			if (Utils::StartsWith(line,  "//")) {
				continue;
			}

			static std::regex regex("maps\\\\mp\\\\(.*)::"s);
			std::smatch m;

			if (std::regex_search(line, m, regex))
			{
				std::string scriptDeclaredName = m[1].str();
				
				Utils::Replace(scriptDeclaredName, "\\", "/");

				if (dumpedSubscripts.contains(scriptDeclaredName))
				{
					continue;
				}

				dumpedSubscripts.insert(scriptDeclaredName);

				// This should be enabled but... some map modders have named their custom scripts with a starting _, so...
				//if (!Utils::StartsWith(scriptDeclaredName, "_")) {
				if (std::find(dumpBlacklist.begin(), dumpBlacklist.end(), scriptDeclaredName) == dumpBlacklist.end()) {
					auto dumpCmd = Utils::VA("dumpRawFile maps/mp/%s.gsc %i", scriptDeclaredName.c_str(), MapDumper::GetZoneIndex());
					Command::Execute(dumpCmd, true);

					GSC::UpgradeGSC(Utils::VA("%s/maps/mp/%s.gsc", AssetHandler::GetExportPath().data(), scriptDeclaredName.c_str()), DumpSubScripts);
				}
				//}
			}
		}

		DumpModels(data);
	}

	void GSC::DumpModels(const std::string& data) 
	{
		static std::regex regex("precacheModel\\(\"([^\"]*)\"\\)"s, std::regex_constants::icase);
		std::smatch m;

		auto stringPtr = data;
		while (std::regex_search(stringPtr, m, regex))
		{
			if (m.size() > 1)
			{
				const auto& modelName = m[1].str();
				auto header = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_XMODEL, modelName.data());

				if (header.model)
				{
					Components::Logger::Print("Dumping additional model %s (mentioned in a precache GSC call)\n", modelName.data());
					AssetHandler::Dump(Game::IW3::XAssetType::ASSET_TYPE_XMODEL, header);

					static auto additionalModelsPath = GetAdditionalModelsListPath();
 					std::ofstream stream(additionalModelsPath, std::ios::binary | std::ios::app);

					if (stream.is_open())
					{
						stream.write((modelName + "\n").data(), modelName.size()+1);
						stream.close();
					}
				}

				stringPtr = m.suffix();
			}
		}
	}

	void GSC::UpgradeGSC(const std::string& filePath, std::function<void(std::string&)> f)
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
		GSC::DumpSubScripts(data);
		GSC::RemoveTeamDeclarations(data);
		GSC::DumpSounds(data);
		GSC::UpgradeCreateFog(data); // It's sometimes in main.gsc! mp_carentan for instance
		GSC::PatchSpecularScale(data);
	}

	void GSC::ConvertMainFXGSC(std::string& data)
	{
		Utils::Replace(data, "\r\n", "\n");
		GSC::PatchReference(data, "maps\\mp\\_fx", "common_scripts\\_fx");
		GSC::PatchReference(data, "maps\\mp\\_createfx", "common_scripts\\_createfx");
		GSC::PatchReference(data, "maps\\mp\\_utility", "common_scripts\\utility");
		GSC::DumpSounds(data);
	}

	void GSC::ConvertMainArtGSC(std::string& data)
	{
		Utils::Replace(data, "\r\n", "\n");
		GSC::UpgradeCreateFog(data);
		GSC::PatchSpecularScale(data); // Everyone has it in main.gsc, except co_hunted, so i guess it could happen to anyone
	}

	void GSC::ConvertFXGSC(std::string& data)
	{
		Utils::Replace(data, "\r\n", "\n");

		GSC::ConvertToStrictCreateFX(data);

		GSC::PatchReference(data, "maps\\mp\\_createfx", "common_scripts\\_createfx");
		GSC::PatchReference(data, "maps\\mp\\_utility", "common_scripts\\utility");
		GSC::DumpSounds(data);

		data = Utils::VA("//_createfx generated. Do not touch!!\n%s", data.data());
	}
	
	void GSC::ConvertToStrictCreateFX(std::string& data)
	{
		static std::regex regex("(?:ent(?:\\.| ).*|\\{|main\\(\\)|#.*|\\})", std::regex_constants::icase);

		std::string newData;

		std::smatch match;

		while (std::regex_search(data, match, regex))
		{
			newData += match[0].str() + "\n";
			data = match.suffix();
		}

		data = newData;
	}

	void GSC::PatchSpecularScale(std::string& data)
	{
		static std::regex regex("setdvar\\( *\"r_specularcolorscale\", *\"([0-9]+\\.*[0-9]*)*\" *\\);"s, std::regex_constants::icase);
		
		std::smatch match;

		if (std::regex_search(data, match, regex))
		{
			auto value = match.str(1);

			auto parsedFloat = std::stof(value);

			// CoD4 speculars, even regenerated, are extremely aggressive for CoD6
			// Everything gets awfully shiny and white!
			// Maps that specify custom specular power explicitely need to have
			//	that value toned down

			parsedFloat = std::log10(parsedFloat) * 0.4f + 1;

			/*
			iw3	iw4
			0.5	0.879588002
			1	1
			2	1.120411998
			3	1.190848502
			5	1.279588002
			*/

			data = std::regex_replace(data, regex, std::format("setdvar(\"r_specularcolorscale\", {}); // Set by iw3xport", parsedFloat));
		}
	}

	void GSC::UpgradeCreateFog(std::string& data)
	{
		static std::regex regex("setExpFog\\(((?:(?:[0-9]*|\\.| )*,*){6}),"s, std::regex_constants::icase);

		data = std::regex_replace(data, regex, "setExpFog($1, 1.0,"); // CoD4 fog is always with 1.0 max opacity
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
