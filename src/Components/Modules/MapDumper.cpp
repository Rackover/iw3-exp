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

	void MapDumper::DumpMap(std::string mapToDump)
	{
		IXModel::Reset();

		MapDumper::mapName = mapToDump;
		bool isSingleplayer = !mapToDump.starts_with("mp_");
		std::string bspName = Utils::VA("maps/%s%s.d3dbsp", isSingleplayer ? "" : "mp/", mapToDump.data());

		static auto additionalModelsFile = GSC::GetAdditionalModelsListPath();
		if (Utils::FileExists(additionalModelsFile))
		{
			// We void it, it might get rewritten anyway
			Utils::WriteFile(additionalModelsFile, "\0");
		}

		Logger::Print("Loading map '%s'...\n", mapToDump.data());
		Command::Execute(Utils::VA("%s %s", isSingleplayer ? "loadzone" : "map", mapToDump.data()), true);
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
		if (!isSingleplayer) // Not sure what this trash code does wrong that upsets singleplayer, but we should probably trash the whole thing just in case
		{
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
		}
		//


		Logger::Print("Exporting ComWorld...\n");
		Command::Execute(Utils::VA("dumpComWorld %s", bspName.data()), true);

		Logger::Print("Exporting GameWorld...\n");
		Command::Execute(Utils::VA("dumpGameWorld %s", bspName.data()), true);

		Logger::Print("Exporting GfxWorld...\n");
		Command::Execute(Utils::VA("dumpGfxWorld %s", bspName.data()), true);

		Logger::Print("Exporting ClipMap...\n");
		Command::Execute(Utils::VA("dumpclipMap_t %s", bspName.data()), true);

		// This is redundant with clipmap but allows exporting more models
		Logger::Print("Exporting Entities...\n");
		Command::Execute(Utils::VA("dumpMapEnts %s", bspName.data()), true);

		Logger::Print("Exporting Vision...\n");
		Command::Execute(Utils::VA("dumpRawFile vision/%s.vision", mapToDump.data()), true);

		Logger::Print("Exporting Sun...\n");
		Command::Execute(Utils::VA("dumpRawFile sun/%s.sun", mapToDump.data()), true);

		Logger::Print("Exporting Compass...\n");
		Command::Execute(Utils::VA("dumpMaterial compass_map_%s", mapToDump.data()), true);

		Logger::Print("Exporting Loadscreen...\n");
		Command::Execute(Utils::VA("dumpMaterial $levelbriefing"), true);

		MapDumper::DumpLoadedGSCs(mapToDump, isSingleplayer);

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

	void MapDumper::DumpLoadedGSCs(std::string mapToDump, bool isSingleplayer)
	{
		Logger::Print("Exporting environment GSCs...\n");
		Command::Execute(Utils::VA("dumpRawFile maps/%s%s.gsc", isSingleplayer ? "" : "mp/", mapToDump.data()), true);
		Command::Execute(Utils::VA("dumpRawFile maps/%s%s_fx.gsc", mapToDump.data()), true);
		Command::Execute(Utils::VA("dumpRawFile maps/createfx/%s_fx.gsc", mapToDump.data()), true);

		Command::Execute(Utils::VA("dumpRawFile maps/createart/%s_art.gsc", mapToDump.data()), true);

		auto convertGsc = Game::Dvar_FindVar("iw3x_convert_gsc");
		if (convertGsc && convertGsc->current.string == "1"s) {
			Logger::Print("Patching GSCs...\n");
			GSC::UpgradeGSC(Utils::VA("%s/maps/createfx/%s_fx.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertFXGSC);
			GSC::UpgradeGSC(Utils::VA("%s/maps/%s%s_fx.gsc", isSingleplayer ? "" : "mp/", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainFXGSC);
			GSC::UpgradeGSC(Utils::VA("%s/maps/createart/%s_art.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainArtGSC);
			GSC::UpgradeGSC(Utils::VA("%s/maps/%s%s.gsc", isSingleplayer ? "" : "mp/", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainGSC);
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
						std::string nameToFind = name;

						if (iw3Type == Game::IW3::ASSET_TYPE_WEAPON)
						{
							// Fix weapon name
							nameToFind = name.substr(4); // Remove iw3_ prefix while seeking
						}

						auto entry = Game::DB_FindXAssetEntry(iw3Type, nameToFind.data());

						if (entry)
						{
							const auto header = entry->entry.asset.header;

							if (header.data && !Game::DB_IsXAssetDefault(iw3Type, nameToFind.data()))
							{
								return AssetHandler::Convert(static_cast<Game::IW3::XAssetType>(iw3Type), header).data;
							}
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

		params.work_directory = DEFAULT_WORK_DIRECTORY;

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
	}

	MapDumper::~MapDumper()
	{
		delete api;
	}
}
