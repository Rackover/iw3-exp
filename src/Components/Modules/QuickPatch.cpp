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

	int QuickPatch::SND_SetDataHook(Game::IW3::MssSound*, char*)
	{
		Game::IW3::LoadedSound*** loadedSoundPtr = reinterpret_cast<Game::IW3::LoadedSound***>(0xE34780);
		auto loadedSound = *(*(loadedSoundPtr));
		
		// We do not dump rightaway, we'll do so when we need to because of soundaliases
		Components::ILoadedSound::DuplicateSoundData(loadedSound);

		// Do not call this or the sounds will actually get loaded
		//return Utils::Hook::Call<int(Game::IW3::MssSound*, char*)>(0x5C8EE0)(sound, data);
		return 0;
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

		// Do not void the vertex declaration on loading
		Utils::Hook::Nop(0x47B466, 5);

#if !DEBUG
		// Do not unload overriden assets - it works in DEBUG, but in RELEASE mode it crashes, and i don't know why.
		Utils::Hook::Set<BYTE>(0x48AEA9, 0xEB);
#endif

		// Intercept SND_SetData
		Utils::Hook(0x4794C2, QuickPatch::SND_SetDataHook, HOOK_CALL).install()->quick();

		Logger::Print("IW3Xport build %s %s\n", __TIME__, __DATE__);

		Command::Add("materialInfoDump", [](Command::Params)
		{
			Game::DB_EnumXAssets_FastFile(Game::IW3::ASSET_TYPE_MATERIAL, [](Game::IW3::XAssetHeader header, void*)
			{
				Logger::Print("%s: %X %X %X\n", header.material->info.name, header.material->info.sortKey & 0xFF, header.material->info.gameFlags.packed & 0xFF, header.material->stateFlags & 0xFF);
			}, nullptr, false);
		});

        Command::Add("loadzone", [](const Command::Params& params)
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
