#include "STDInclude.hpp"

namespace Components
{
	void QuickPatch::PerformInit()
	{
		Command::Execute("set dedicated 1", true);
	}

	__declspec(naked) void QuickPatch::OnInitStub()
	{
		__asm
		{
			pushad
			call QuickPatch::PerformInit
			popad

			push 4FD740h
			retn
		}
	}

	QuickPatch::QuickPatch()
	{
		// enable commandline
		//Utils::Hook::Nop(0x57760E, 5);

		// set default commandline
		//Utils::Hook::SetString(0xCC157E0, "+set dedicated 1", 1023);

		// Set fs_game
		Utils::Hook::Set<const char*>(0x55E509, "mods/iw3-exp");

		Utils::Hook(0x4FF20A, QuickPatch::OnInitStub, HOOK_CALL).install()->quick();

		// Force debug logging
		Utils::Hook::Nop(0x4FCB9D, 8);

		// Enable console log
		Utils::Hook::Nop(0x4FCBA3, 2);

		// Ignore hardware changes
		Utils::Hook::Set<DWORD>(0x57676B, 0xC359C032);
		Utils::Hook::Set<DWORD>(0x576691, 0xC359C032);

		// Remove improper quit popup
		Utils::Hook::Set<BYTE>(0x5773F6, 0xEB);

		// Fix microphone shit
		Utils::Hook::Set<BYTE>(0x57AB09, 0xEB);
		Utils::Hook::Nop(0x4ED366, 5);

		Command::Add("materialInfoDump", [](Command::Params)
		{
			Game::DB_EnumXAssets_FastFile(Game::ASSET_TYPE_MATERIAL, [](Game::IW3::XAssetHeader header, void*)
			{
				Logger::Print("%s: %X %X %X\n", header.material->info.name, header.material->info.sortKey & 0xFF, header.material->info.gameFlags & 0xFF, header.material->stateFlags & 0xFF);
			}, nullptr, false);
		});

		Command::Add("dumpMap", [](Command::Params params)
		{
			if (params.Length() < 2) return;
			std::string mapname = params[1];
			std::string bspname = Utils::VA("maps/mp/%s.d3dbsp", mapname.data());

			Logger::Print("IW3Xport build %s %s\n", __TIME__, __DATE__);

			Logger::Print("Loading map '%s'...\n", mapname.data());
			Command::Execute(Utils::VA("map %s", mapname.data()), true);

			Logger::Print("Exporting ComWorld...\n");
			Command::Execute(Utils::VA("dumpComWorld %s", bspname.data()), true);

			Logger::Print("Exporting GfxWorld...\n");
			Command::Execute(Utils::VA("dumpGfxWorld %s", bspname.data()), true);

			Logger::Print("Exporting ClipMap...\n");
			Command::Execute(Utils::VA("dumpclipMap_t %s", bspname.data()), true);

			Logger::Print("Exporting Vision...\n");
			Command::Execute(Utils::VA("dumpRawFile vision/%s.vision", mapname.data()), true);

			Logger::Print("Exporting Sun...\n");
			Command::Execute(Utils::VA("dumpRawFile sun/%s.sun", mapname.data()), true);

			Logger::Print("Exporting Compass...\n");
			Command::Execute(Utils::VA("dumpMaterial compass_map_%s", mapname.data()), true);

			Logger::Print("Exporting Loadscreen...\n");
			Command::Execute(Utils::VA("dumpGfxImage loadscreen_%s", mapname.data()), true);

			Logger::Print("Exporting environment GSCs...\n");
			Command::Execute(Utils::VA("dumpRawFile maps/mp/%s_fx.gsc", mapname.data()), true);
			Command::Execute(Utils::VA("dumpRawFile maps/createfx/%s_fx.gsc", mapname.data()), true);
			Command::Execute(Utils::VA("dumpRawFile maps/createart/%s_art.gsc", mapname.data()), true);

			Logger::Print("Patching GSCs...\n");
			auto patchGSC = [](std::string file)
			{
				auto patchReference = [](std::string& data, std::string _old, std::string _new)
				{
					Utils::Replace(data, _old, _new);

					// Remove double includes
					auto lines = Utils::Explode(data, '\n');

					int count = 0;
					std::string newData;
					for(auto& line : lines)
					{
						if (line == Utils::VA("#include %s;", _new.data()) && count++ > 0)
						{
							continue;
						}

						newData.append(line);
						newData.append("\n");
					}

					data = newData;
				};

				if (Utils::FileExists(file))
				{
					std::string data = Utils::ReadFile(file);
					Utils::Replace(data, "\r\n", "\n");
					patchReference(data, "maps\\mp\\_utility", "common_scripts\\utility");
					patchReference(data, "maps\\mp\\_createfx", "common_scripts\\_createfx");
					patchReference(data, "maps\\mp\\_fx", "common_scripts\\_fx");
					Utils::WriteFile(file, data);
				}
			};

			patchGSC(Utils::VA("%s/maps/createfx/%s_fx.gsc", AssetHandler::GetExportPath().data(), mapname.data()));
			patchGSC(Utils::VA("%s/maps/mp/%s_fx.gsc", AssetHandler::GetExportPath().data(), mapname.data()));

			// Search zone index
			int zoneIndex = 0;
			for (; zoneIndex < 32; ++zoneIndex)
			{
				if(Game::g_zones[zoneIndex].name == mapname)
				{
					break;
				}
			}

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

			Logger::Print("Map '%s' successfully exported.\n", mapname.data());
		});

        Command::Add("loadzone", [](Command::Params params)
        {
            if (params.Length() < 2) return;
            std::string zone = params[1];

            Game::XZoneInfo info;
            info.name = zone.data();
            info.allocFlags = 0;
            info.freeFlags = 0;

            Game::DB_LoadXAssets(&info, 1, 0);

        });
	}

	QuickPatch::~QuickPatch()
	{
		
	}
}
