#include "STDInclude.hpp"

namespace Components
{

	std::string MapDumper::mapName;
	int MapDumper::zoneIndex;
	iw4of::api* MapDumper::api;

	iw4of::api* MapDumper::GetApi() {
		return MapDumper::api;
	}

	std::string MapDumper::GetMapName() {
		return MapDumper::mapName;
	}

	int MapDumper::GetZoneIndex() {
		return MapDumper::zoneIndex;
	}

	void MapDumper::DumpMpTypes(std::ofstream& zoneSource, std::vector<std::string> mpTypes)
	{
		for (const std::string& mpType : mpTypes)
		{
			const auto& path = std::format("mptype/{}.gsc", mpType);

			Game::IW3::XAssetHeader mpTypeFile = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_RAWFILE, path.data());
			if (!mpTypeFile.data)
			{
				continue;
			}

			Components::Logger::Print("Parsing mptype %s\n", mpType.data());
			zoneSource << "rawfile," << path << "\n";
			Command::Execute(std::format("dumpRawFile {}", path), true);


			std::vector<std::string> characterFiles{};

			// Parse mptype
			{
				std::string contents = mpTypeFile.rawfile->buffer;
				static std::regex subscriptCatcher("((?:character|xmodelalias)\\\\(?:.+))::");
				{
					std::smatch matches;
					std::string::const_iterator searchStart(contents.cbegin());

					while (std::regex_search(searchStart, contents.cend(), matches, subscriptCatcher))
					{
						if (matches.size() > 1)
						{
							const auto& match = matches[1];
							auto scriptName = std::format("{}.gsc", match.str());
							std::replace(scriptName.begin(), scriptName.end(), '\\', '/');

							Game::IW3::XAssetHeader characterFile = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_RAWFILE, scriptName.data());

							if (characterFile.data)
							{
								characterFiles.push_back(characterFile.rawfile->buffer);

								Command::Execute(std::format("dumpRawFile {}", scriptName), true);
								zoneSource << "rawfile," << scriptName << "\n";
								Components::Logger::Print("Preparing to dump character %s\n", scriptName.data());
							}
						}

						searchStart = matches.suffix().first;
					}
				}
			}

			// Parse character files
			{
				for (const auto& contents : characterFiles)
				{
					static std::regex modelCatcher("precache.odel\\( ?\"(.*)\" ?\\);");
					{
						std::smatch matches;
						std::string::const_iterator searchStart(contents.cbegin());

						while (std::regex_search(searchStart, contents.cend(), matches, modelCatcher))
						{
							if (matches.size() > 1)
							{
								const auto& match = matches[1];
								auto modelName = match.str();
								zoneSource << "xmodel," << modelName << "\n";
								Command::Execute(std::format("dumpXModel {}", modelName), true);
							}

							searchStart = matches.suffix().first;
						}
					}
				}
			}
		}

		Components::Logger::Print("Done with mptypes\n");
	}

	void MapDumper::DumpTeams()
	{
		struct mpTypes
		{
			std::string sniper;
			std::string support;
			std::string assault;
			std::string recon;
			std::string specops;
			std::string riot;
		};

		struct teamDef
		{
			std::string originalName;
			std::string iw4name;
			std::string iw4fallback;
			std::string localizedName;
			std::string localizedShortName;
			std::string eliminatedName;
			std::string forfeitedName;
			std::string factionImage;
			std::string factionImageFade; // Duplicate normal image
			std::string voicePrefix;	  //
			std::string spawnTheme;
			std::string victoryTheme;
			std::string flagModel;
			std::string flagCarryModel;
			std::string objPointFlagMaterial;
			std::string flagFX;
			Game::vec3_t color;
			std::string headIcon;	// Absent from IW3
			std::string plasticCaseModel; // Absent from IW3
			
			std::string takeFromMap;
			mpTypes mptypes;
		};

		const teamDef finalTeams[] = {
			// ORIGINAL NAME			NAME			FALLBACK			LOC NAME			LOC SHORT				ELIMINATED					FORFEITED				MAT					MAT_FADE			VOICE	SPAWN_THEME		VICTORY_THEME		FLAG						FLAG_CARRY				OBJPOINT						FLAG_FX					COLOR					HEADICON			PLASTIC_CASE				// MAP			// MPTYPES
			{ "marines",	 "iw3_usmc_desert",			"us_army",			"MPUI_MARINES",		"MPUI_MARINES_SHORT",	"MP_MARINES_ELIMINATED",	"MP_MARINES_FORFEITED",	"faction_128_usmc", "faction_128_usmc_fade",	"US_",	"mp_spawn_usa",	"mp_victory_usa", "prop_flag_neutral",			"prop_flag_neutral",	"objpoint_flag_american",		"ui_flagbase_black",	{0.0f, 0.0f,  0.0f},	"headicon_rangers",	"com_plasticcase_beige_big", "mp_bog",		{"mptype_ally_sniper",				"mptype_ally_support",			"mptype_ally_rifleman",			"mptype_ally_engineer",				"mptype_ally_cqb",			"mptype_us_army_riot" }},
			{ "opfor",		 "iw3_opfor_desert",		"opforce_composite","MPUI_OPFOR",		"MPUI_OPFOR_SHORT",		"MP_OPFOR_ELIMINATED",		"MP_OPFOR_FORFEITED",		"faction_128_arab",	"faction_128_arab_fade",	"AB_",	"mp_spawn_opfor",	"mp_victory_opfor", "prop_flag_neutral",	"prop_flag_neutral",	"objpoint_flag_opfor",			"ui_flagbase_black", {0.6f,  0.57f, 0.41f},	"headicon_arab",	"com_plasticcase_beige_big", "mp_bog",		{"mptype_axis_sniper",		"mptype_axis_support",	 "mptype_axis_rifleman",	"mptype_axis_engineer",		"mptype_axis_cqb",	"mptype_opforce_comp_riot" }},
//			{ "sas",		 "iw3_sas_urban",			"seals_udt",	"MPUI_SAS",			"MPUI_SAS_SHORT",		"MP_SAS_ELIMINATED",		"MP_SAS_FORFEITED",		"faction_128_sas",	"faction_128_sa_fades",	"UK_",	"mp_spawn_sas",	"mp_victory_sas", "prop_flag_neutral",			"prop_flag_neutral",	"objpoint_flag_british",		"ui_flagbase_black",	{0.6f,  0.64f, 0.69f},	"headicon_taskforce141",	"com_plasticcase_beige_big", "mp_carentan",		{"mptype_ally_urban_sniper",		"mptype_ally_urban_support",	"mptype_ally_urban_assault",	"mptype_ally_urban_recon",			"mptype_ally_urban_specops",	"mptype_seal_udt_riot" }},
//			{ "russian",	 "iw3_spetsnaz_urban",		"opforce_airborne",	"MPUI_SPETSNAZ",	"MPUI_SPETSNAZ_SHORT",	"MP_SPETSNAZ_ELIMINATED",	"MP_SPETSNAZ_FORFEITED",	"faction_128_ussr",	"faction_128_ussr_fade",	"RU_",	"mp_spawn_soviet",	"mp_victory_soviet", "prop_flag_neutral",	"prop_flag_neutral",	"objpoint_flag_russian",		"ui_flagbase_black", {0.58f,  0.28f, 0.28f},"headicon_ussr",	"com_plasticcase_beige_big", "mp_carentan",		{"mptype_axis_urban_sniper",		"mptype_axis_urban_support",	 "mptype_axis_urban_assault",	"mptype_axis_urban_engineer",		"mptype_axis_urban_cqb",	"mptype_opforce_airborne_riot" }},
//			{ "sas",		 "iw3_sas_woodland",		"socom_141_forest",	"MPUI_SAS",			"MPUI_SAS_SHORT",		"MP_SAS_ELIMINATED",		"MP_SAS_FORFEITED",		"faction_128_sas",	"faction_128_sas_fade",	"UK_",	"mp_spawn_sas",	"mp_victory_sas", "prop_flag_neutral",			"prop_flag_neutral",	"objpoint_flag_british",		"ui_flagbase_black",	{0.6f,  0.64f, 0.69f},	"headicon_taskforce141",	"com_plasticcase_beige_big", "mp_pipeline",	{"mptype_ally_woodland_sniper",	"mptype_ally_woodland_support",	"mptype_ally_woodland_assault",	"mptype_ally_woodland_recon",		"mptype_ally_woodland_specops",	"mptype_tf141_forest_riot" }},
//			{ "russian",	 "iw3_spetsnaz_woodland",	"opforce_airborne",	"MPUI_SPETSNAZ",	"MPUI_SPETSNAZ_SHORT",	"MP_SPETSNAZ_ELIMINATED",	"MP_SPETSNAZ_FORFEITED",	"faction_128_ussr",	"faction_128_ussr_fade",	"RU_",	"mp_spawn_soviet",	"mp_victory_soviet", "prop_flag_neutral",	"prop_flag_neutral",	"objpoint_flag_russian",		"ui_flagbase_black", {0.58f,  0.28f, 0.28f},"headicon_ussr",	"com_plasticcase_beige_big", "mp_pipeline",	{"mptype_axis_woodland_sniper",		"mptype_axis_woodland_support",	 "mptype_axis_woodland_rifleman",	"mptype_axis_woodland_engineer",		"mptype_axis_woodland_cqb",	"mptype_opforce_airborne_riot" }}
		};

		const auto& basePath = AssetHandler::GetExportPath();

		for (const auto& team : finalTeams)
		{
			const std::string& exportPath = std::format("{}/{}", basePath, team.iw4name);
			Utils::CreateDir(exportPath);
			GetApi()->set_work_path(exportPath);

			std::string mapToDump = team.takeFromMap;

			if (MapDumper::mapName != mapToDump)
			{
				MapDumper::mapName = mapToDump;
				Logger::Print("Loading map '%s'...\n", mapToDump.data());
				Command::Execute(Utils::VA("map %s", mapToDump.data()), true);
			}

			std::string lowerPrefix = team.voicePrefix;
			for (auto& c : lowerPrefix)
			{
				c = static_cast<char>(tolower(c));
			}

			const auto newPrefix = std::format("IW3_{}", team.voicePrefix);
			const auto newLowPrefix = std::format("iw3_{}", lowerPrefix);

			const auto& newVictoryTheme = std::format("IW3_{}", team.victoryTheme);
			const auto& newSpawnTheme = std::format("IW3_{}", team.spawnTheme);

			{
				std::ofstream zoneSource(std::format("{}/{}.csv", exportPath, team.iw4name));
				zoneSource << "# IW3xport - generated by Louve@Xlabs\n\n";

				DumpMpTypes(zoneSource, { team.mptypes.assault, team.mptypes.sniper, team.mptypes.support, team.mptypes.recon, team.mptypes.specops });

				zoneSource << "material," << team.factionImage << "\n";
				Command::Execute(Utils::VA("dumpMaterial %s", team.factionImage.data()), true);

				zoneSource << "material," << team.factionImageFade << "\n";
				// Copy the same image - IW3 doesnt have fade
				// Need to rename it inside too...
				{
					std::ifstream readFile(std::format("{}/materials/{}.iw4x.json", exportPath, team.factionImage));
					std::ofstream outFile(std::format("{}/materials/{}.iw4x.json", exportPath, team.factionImageFade));
					std::string readout;
					std::string search = std::format("    \"name\": \"{}\",", team.factionImage);
					std::string replace = std::format("    \"name\": \"{}\",", team.factionImageFade);
					while (getline(readFile, readout)) {
						if (readout == search) {
							outFile << replace << "\n";
						}
						else {
							outFile << readout << "\n";
						}
					}
				}

				// dump all voice sounds using prefix (easier than parsing GSC)
				{
					std::vector<std::string> soundsToDump{};

					Game::DB_EnumXAssetEntries(Game::IW3::XAssetType::ASSET_TYPE_SOUND, [&team, &soundsToDump, &lowerPrefix](Game::IW3::XAssetEntryPoolEntry* poolEntry) {
						if (poolEntry)
						{
							auto header = poolEntry->entry.asset.header;
							if (header.data && header.sound->count > 0 && header.sound->count < 32)
							{
								std::string name = header.sound->aliasName;

								if (name.starts_with(team.voicePrefix) || name.starts_with(lowerPrefix))
								{
									soundsToDump.push_back(name);
								}
							}
						}
						}, false);


					for (const auto& soundName : soundsToDump)
					{
						const auto newPrefixedName = std::format("{}{}", newPrefix, soundName.substr(3));
						zoneSource << "sound," << newPrefixedName << "\n";
						Command::Execute(Utils::VA("dumpSound %s", soundName.data()), true);

						// Rename it once dumped 
						const std::string outputPath = std::format("{}/sounds/{}.json", exportPath, soundName);
						const std::string newPath = std::format("{}/sounds/{}.json", exportPath, newPrefixedName);
						if (Utils::FileExists(outputPath))
						{
							if (Utils::FileExists(newPath))
							{
								std::filesystem::remove(newPath);
							}

							std::filesystem::rename(outputPath, newPath);
						}
					}
				}

				zoneSource << "sound," << newSpawnTheme << "\n";
				Command::Execute(Utils::VA("dumpSound %s", team.spawnTheme.data()), true);
				{
					// Rename it once dumped 
					const std::string outputPath = std::format("{}/sounds/{}.json", exportPath, team.spawnTheme);
					const std::string newPath = std::format("{}/sounds/{}.json", exportPath, newSpawnTheme);
					if (Utils::FileExists(outputPath))
					{
						if (Utils::FileExists(newPath))
						{
							std::filesystem::remove(newPath);
						}

						std::filesystem::rename(outputPath, newPath);

						const auto& iw4ExpectedPath = std::format("{}/sounds/{}{}.json", exportPath, newLowPrefix, "spawn_music");
						std::filesystem::remove(iw4ExpectedPath);
						std::filesystem::copy(newPath, iw4ExpectedPath);
					}
				}

				zoneSource << "sound," << newVictoryTheme << "\n";
				Command::Execute(Utils::VA("dumpSound %s", team.victoryTheme.data()), true);
				{
					// Rename it once dumped 
					const std::string outputPath = std::format("{}/sounds/{}.json", exportPath, team.victoryTheme);
					const std::string newPath = std::format("{}/sounds/{}.json", exportPath, newVictoryTheme);
					if (Utils::FileExists(outputPath))
					{
						if (Utils::FileExists(newPath))
						{
							std::filesystem::remove(newPath);
						}

						std::filesystem::rename(outputPath, newPath);
						const auto& iw4ExpectedPath = std::format("{}/sounds/{}{}.json", exportPath, newLowPrefix, "victory_music");
						std::filesystem::remove(iw4ExpectedPath);
						std::filesystem::copy(newPath, iw4ExpectedPath);
					}
				}

				zoneSource << "xmodel," << team.flagCarryModel << "\n";
				Command::Execute(Utils::VA("dumpXModel %s", team.flagCarryModel.data()), true);

				zoneSource << "fx," << team.flagFX << "\n";
				Command::Execute(Utils::VA("dumpFxEffectDef %s", team.flagFX.data()), true);

				zoneSource << "material," << team.objPointFlagMaterial << "\n";
				Command::Execute(Utils::VA("dumpMaterial %s", team.objPointFlagMaterial.data()), true);

				zoneSource << "material," << team.headIcon << "\n";
				Command::Execute(Utils::VA("dumpMaterial %s", team.headIcon.data()), true);

				zoneSource << "xmodel," << team.plasticCaseModel << "\n";
				Command::Execute(Utils::VA("dumpXModel %s", team.plasticCaseModel.data()), true);

				zoneSource << "xmodel," << team.flagModel << "\n";
				Command::Execute(Utils::VA("dumpXModel %s", team.flagModel.data()), true);

			}

			Utils::WriteFile(std::format("{}/iw4_voice_fallback.txt", exportPath), lowerPrefix);

			Utils::WriteFile(std::format("{}/iw4_team_fallback.txt", exportPath), team.iw4fallback);

			// GSC
			Utils::WriteFile(
				std::format("{}/_teams.gsc", exportPath),
				std::format("\n\
							case \"{0}\":\n\
								mptype\\{1}::precache();\n\
								mptype\\{2}::precache();\n\
								mptype\\{3}::precache();\n\
								mptype\\{4}::precache();\n\
								mptype\\{5}::precache();\n\
								mptype\\{6}::precache();\n\
\n\
								game[team + \"_model\"][\"SNIPER\"] = mptype\\{1}::main;\n\
								game[team + \"_model\"][\"LMG\"] = mptype\\{2}::main;\n\
								game[team + \"_model\"][\"ASSAULT\"] = mptype\\{3}::main;\n\
								game[team + \"_model\"][\"SHOTGUN\"] = mptype\\{4}::main;\n\
								game[team + \"_model\"][\"SMG\"] = mptype\\{5}::main;\n\
								game[team + \"_model\"][\"RIOT\"] = mptype\\{6}::main;\n\
\n\
								break; \n\
				", team.iw4name, team.mptypes.sniper, team.mptypes.support, team.mptypes.assault, team.mptypes.specops, team.mptypes.recon, team.mptypes.riot)
			);


			// STR
			{
				std::ofstream strFile(std::format("{}/{}.str", exportPath, team.iw4name));
				if (strFile.is_open())
				{
					strFile << "REFERENCE			" << team.localizedName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.localizedName.data(), "", 0) << "\n\n";
					strFile << "REFERENCE			" << team.localizedShortName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.localizedShortName.data(), "", 0) << "\n\n";
					strFile << "REFERENCE			" << team.eliminatedName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.eliminatedName.data(), "", 0) << "\n\n";
					strFile << "REFERENCE			" << team.forfeitedName << "\n" << "LANG_ENGLISH		" << Game::SEH_LocalizeTextMessage(team.forfeitedName.data(), "", 0) << "\n\n";
				}
			}


			// Factiontable
			Utils::WriteFile(
				std::format("{}/factionTable.csv", exportPath),
				std::format
				(
					"{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}",
					team.iw4name,team.localizedName,team.localizedShortName,team.eliminatedName,team.forfeitedName,team.factionImage,team.factionImageFade,newPrefix,newSpawnTheme,newVictoryTheme,team.flagModel,team.flagCarryModel,team.objPointFlagMaterial,team.flagFX,team.color[0],team.color[1],team.color[2],team.headIcon,team.plasticCaseModel
				)
			);
		}

	/*	Game::DB_EnumXAssetEntries(Game::IW3::XAssetType::ASSET_TYPE_MATERIAL, [](Game::IW3::XAssetEntryPoolEntry* poolEntry) {
			if (poolEntry)
			{

			}
			}, false);*/



		//Command::Execute(Utils::VA("loadzone %s", map), true);

	}

	void MapDumper::DumpMap(std::string mapToDump)
	{
		MapDumper::mapName = mapToDump;
		std::string bspName = Utils::VA("maps/mp/%s.d3dbsp", mapToDump.data());

		static auto additionalModelsFile = GSC::GetAdditionalModelsListPath();
		if (Utils::FileExists(additionalModelsFile))
		{
			// We void it, it might get rewritten anyway
			Utils::WriteFile(additionalModelsFile, "\0");
		}

		Logger::Print("Loading map '%s'...\n", mapToDump.data());
		Command::Execute(Utils::VA("map %s", mapToDump.data()), true);
		Command::Execute(Utils::VA("loadzone %s_load", mapToDump.data()), true);

		// Search zone index
		int myZoneIndex = 0;
		for (; myZoneIndex < 32; ++myZoneIndex)
		{
			if (Game::g_zones[myZoneIndex].name == mapToDump)
			{
				break;
			}
		}

		MapDumper::zoneIndex = myZoneIndex;

		Logger::Print("Exporting all sounds...\n");
		// Ultra-heavy sound dumping
		// It doesn't have to be this way: We could instead gather every soundname that is
		// - In the zone (that's already the case)
		// - Appears in a GSC (createFX)
		// - Appears in a map ent (is that even possible?)
		// and dump only these! 
		Game::DB_EnumXAssetEntries(Game::IW3::XAssetType::ASSET_TYPE_SOUND, [myZoneIndex](Game::IW3::XAssetEntryPoolEntry* poolEntry) {
			if (poolEntry)
			{
				auto entry = &poolEntry->entry;
				if (entry->zoneIndex == myZoneIndex && entry->inuse == 0 && entry->asset.header.sound && entry->asset.header.sound->aliasName)
				{
					try
					{
						if (Utils::StartsWith(entry->asset.header.sound->aliasName, "weap"))
						{
							return;
						}
						else if (Utils::StartsWith(entry->asset.header.sound->aliasName, "melee"))
						{
							return;
						}
						else if (Utils::StartsWith(entry->asset.header.sound->aliasName, "c4"))
						{
							return;
						}
						else if (entry->asset.header.sound->head) {
							if (entry->asset.header.sound->head->soundFile)
							{
								auto soundFileName = entry->asset.header.sound->head->soundFile->type == Game::snd_alias_type_t::SAT_LOADED ?
									entry->asset.header.sound->head->soundFile->u.loadSnd->name :
									entry->asset.header.sound->head->soundFile->u.streamSnd.filename.info.raw.dir;

								if (Utils::StartsWith(soundFileName, "vehicles"))
								{
									return;
								}

								if (Utils::StartsWith(soundFileName, "voiceovers"))
								{
									return;
								}
							}
						}

						//Components::Logger::Print("%d => %s\n", entry->zoneIndex, entry->asset.header.sound->aliasName);
						auto converted = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_SOUND, entry->asset.header);
						GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_SOUND, converted.data);
					}
					catch (const std::exception&)
					{
						// There's a good chance DB_EnumDXAssetEntries just gave me garbage data
						// No need to make a fuzz
					}
				}
			}
			}, false);

		//

		Logger::Print("Exporting ComWorld...\n");
		Command::Execute(Utils::VA("dumpComWorld %s", bspName.data()), true);

		Logger::Print("Exporting GameWorld...\n");
		Command::Execute(Utils::VA("dumpGameWorld %s", bspName.data()), true);

		Logger::Print("Exporting GfxWorld...\n");
		Command::Execute(Utils::VA("dumpGfxWorld %s", bspName.data()), true);

		Logger::Print("Exporting ClipMap...\n");
		Command::Execute(Utils::VA("dumpclipMap_t %s", bspName.data()), true);

		Logger::Print("Exporting Vision...\n");
		Command::Execute(Utils::VA("dumpRawFile vision/%s.vision", mapToDump.data()), true);

		Logger::Print("Exporting Sun...\n");
		Command::Execute(Utils::VA("dumpRawFile sun/%s.sun", mapToDump.data()), true);

		Logger::Print("Exporting Compass...\n");
		Command::Execute(Utils::VA("dumpMaterial compass_map_%s", mapToDump.data()), true);

		Logger::Print("Exporting Loadscreen...\n");
		Command::Execute(Utils::VA("dumpGfxImage loadscreen_%s", mapToDump.data()), true);

		MapDumper::DumpLoadedGSCs(mapToDump);

		if (myZoneIndex < 32)
		{
			Logger::Print("Exporting FXs...\n");

			// Dump all available fx
			Game::DB_EnumXAssetEntries(Game::IW3::ASSET_TYPE_FX, [myZoneIndex](Game::IW3::XAssetEntryPoolEntry* entry)
				{
					if (entry->entry.zoneIndex == myZoneIndex)
					{
						std::string name = Game::DB_GetXAssetNameHandlers[entry->entry.asset.type](&entry->entry.asset.header);
						Command::Execute(Utils::VA("dumpFxEffectDef %s", name.data()), true);
					}
				}, false);
		}

	}

	void MapDumper::DumpLoadedGSCs(std::string mapToDump)
	{
		Logger::Print("Exporting environment GSCs...\n");
		Command::Execute(Utils::VA("dumpRawFile maps/mp/%s.gsc", mapToDump.data()), true);
		Command::Execute(Utils::VA("dumpRawFile maps/mp/%s_fx.gsc", mapToDump.data()), true);
		Command::Execute(Utils::VA("dumpRawFile maps/createfx/%s_fx.gsc", mapToDump.data()), true);

		Command::Execute(Utils::VA("dumpRawFile maps/createart/%s_art.gsc", mapToDump.data()), true);

		auto convertGsc = Game::Dvar_FindVar("iw3x_convert_gsc");
		if (convertGsc && convertGsc->current.string == "1"s) {
			Logger::Print("Patching GSCs...\n");
			GSC::UpgradeGSC(Utils::VA("%s/maps/createfx/%s_fx.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertFXGSC);
			GSC::UpgradeGSC(Utils::VA("%s/maps/mp/%s_fx.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainFXGSC);
			GSC::UpgradeGSC(Utils::VA("%s/maps/createart/%s_art.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainArtGSC);
			GSC::UpgradeGSC(Utils::VA("%s/maps/mp/%s.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainGSC);
		}
	}

	std::string MapDumper::APIFileRead(const std::string& filename)
	{
		if (filename.ends_with(".iwi"))
		{
			return IGfxImage::ConvertIWIOnTheFly(filename);
		}

		return FileSystem::File(filename).GetBuffer();
	}

	iw4of::params_t MapDumper::GetParams()
	{
		auto params = iw4of::params_t();

		params.write_only_once = true;

		params.fs_read_file = APIFileRead;

		params.get_from_string_table = [](unsigned int index)
		{
			return Game::SL_ConvertToString(index);
		};

		params.find_other_asset = [](int type, const std::string& name)
		{
			for (const auto& kv : AssetHandler::TypeTable)
			{
				if (kv.second == type)
				{
					auto iw3Type = kv.first;
					auto header = Game::DB_FindXAssetHeader(iw3Type, name.data());

					if (header.data)
					{
						return AssetHandler::Convert(static_cast<Game::IW3::XAssetType>(iw3Type), header).data;
					}

					return static_cast<void*>(nullptr);
				}
			}

			return static_cast<void*>(nullptr);
		};

		params.print = [](int level, const std::string& message)
		{
			if (level)
			{
				Logger::Error(message.data());
				assert(false);
			}
			else
			{
				Logger::Print(message.data());
			}
		};

		params.work_directory = "iw3xport_out/default";

		return params;
	}

	MapDumper::MapDumper()
	{

		api = new iw4of::api(GetParams());

		Command::Add("dumpMap", [](const Command::Params& params)
			{
				if (params.Length() < 2) return;
				std::string mapname = params[1];

				api->set_work_path(AssetHandler::GetExportPath());

				MapDumper::DumpMap(mapname);
				Logger::Print("Map '%s' successfully exported.\n", mapname.data());
			});

		Command::Add("dumpTeams", [](const Command::Params&)
			{
				api->set_work_path(AssetHandler::GetExportPath());

				MapDumper::DumpTeams();
				Logger::Print("Teams successfully exported.\n");
			});
	}

	MapDumper::~MapDumper()
	{
		delete api;
	}
}
