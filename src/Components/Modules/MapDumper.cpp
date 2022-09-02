#include "STDInclude.hpp"

namespace Components
{
	std::string MapDumper::mapName;

	std::string MapDumper::GetMapName() {
		return MapDumper::mapName;
	}

	void MapDumper::DumpMap(std::string mapToDump)
	{
#define EXTEND_CULLING 1

		MapDumper::mapName = mapToDump;
		std::string bspName = Utils::VA("maps/mp/%s.d3dbsp", mapToDump.data());

		Logger::Print("Loading map '%s'...\n", mapToDump.data());
		Command::Execute(Utils::VA("map %s", mapToDump.data()), true);
		Command::Execute(Utils::VA("loadzone %s_load", mapToDump.data()), true);

		// Search zone index
		int zoneIndex = 0;
		for (; zoneIndex < 32; ++zoneIndex)
		{
			if (Game::g_zones[zoneIndex].name == mapToDump)
			{
				break;
			}
		}

		Logger::Print("Exporting all sounds...\n");
		// Ultra-heavy sound dumping
		// It doesn't have to be this way: We could instead gather every soundname that is
		// - In the zone (that's already the case)
		// - Appears in a GSC (createFX)
		// - Appears in a map ent (is that even possible?)
		// and dump only these! 
		Game::DB_EnumXAssetEntries(Game::XAssetType::ASSET_TYPE_SOUND, [zoneIndex](Game::IW3::XAssetEntry* entry) {
			if (entry->zoneIndex == zoneIndex && entry->asset.header.sound && entry->asset.header.sound->aliasName)
			{
				try
				{
					if (Utils::StartsWith(entry->asset.header.sound->aliasName, "weap"))
					{
					}
					else if (Utils::StartsWith(entry->asset.header.sound->aliasName, "melee"))
					{
					}
					else if (Utils::StartsWith(entry->asset.header.sound->aliasName, "c4"))
					{
					}
					else
					{
						//Components::Logger::Print("%d => %s\n", entry->zoneIndex, entry->asset.header.sound->aliasName);
						AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_SOUND, entry->asset.header);
					}
				}
				catch (const std::exception&)
				{
					// There's a good chance DB_EnumDXAssetEntries just gave me garbage data
					// No need to make a fuzz
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

		if (zoneIndex < 32)
		{
			Logger::Print("Exporting FXs...\n");

			// Dump all available fx
			Game::DB_EnumXAssetEntries(Game::ASSET_TYPE_FX, [zoneIndex](Game::IW3::XAssetEntry* entry)
				{
					if (entry->zoneIndex == zoneIndex)
					{
						std::string name = Game::DB_GetXAssetNameHandlers[entry->asset.type](&entry->asset.header);
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

		Logger::Print("Patching GSCs...\n");
		GSC::UpgradeGSC(Utils::VA("%s/maps/createfx/%s_fx.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertFXGSC);
		GSC::UpgradeGSC(Utils::VA("%s/maps/mp/%s_fx.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainFXGSC);
		GSC::UpgradeGSC(Utils::VA("%s/maps/createart/%s_art.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainArtGSC);
		GSC::UpgradeGSC(Utils::VA("%s/maps/mp/%s.gsc", AssetHandler::GetExportPath().data(), mapToDump.data()), GSC::ConvertMainGSC);
	}

	MapDumper::MapDumper()
	{
		Command::Add("dumpMap", [](Command::Params params)
			{
				if (params.Length() < 2) return;
				std::string mapname = params[1];
				MapDumper::DumpMap(mapname);
				Logger::Print("Map '%s' successfully exported.\n", mapname.data());
			});
	}

	MapDumper::~MapDumper()
	{

	}
}
