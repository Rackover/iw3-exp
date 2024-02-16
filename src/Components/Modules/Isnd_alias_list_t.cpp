#include "STDInclude.hpp"

namespace Components
{
	int Isnd_alias_list_t::channelLookupTable[Game::IW3::SND_CHANNEL_COUNT]
	{
		Game::IW4::SND_CHANNEL_PHYSICS,
		Game::IW4::SND_CHANNEL_AUTO,
		Game::IW4::SND_CHANNEL_AUTO2,
		Game::IW4::SND_CHANNEL_AUTODOG,
		Game::IW4::SND_CHANNEL_BULLETIMPACT,
		Game::IW4::SND_CHANNEL_BULLETWHIZBY,
		Game::IW4::SND_CHANNEL_ELEMENT,
		Game::IW4::SND_CHANNEL_AUTO2D,
		Game::IW4::SND_CHANNEL_VEHICLE,
		Game::IW4::SND_CHANNEL_VEHICLELIMITED,
		Game::IW4::SND_CHANNEL_MENU,
		Game::IW4::SND_CHANNEL_BODY,
		Game::IW4::SND_CHANNEL_BODY2D,
		Game::IW4::SND_CHANNEL_RELOAD,
		Game::IW4::SND_CHANNEL_RELOAD2D,
		Game::IW4::SND_CHANNEL_ITEM,
		Game::IW4::SND_CHANNEL_EFFECTS1,
		Game::IW4::SND_CHANNEL_EFFECTS2,
		Game::IW4::SND_CHANNEL_WEAPON,
		Game::IW4::SND_CHANNEL_WEAPON2D,
		Game::IW4::SND_CHANNEL_NONSHOCK,
		Game::IW4::SND_CHANNEL_VOICE,
		Game::IW4::SND_CHANNEL_LOCAL,
		Game::IW4::SND_CHANNEL_LOCAL2,
		Game::IW4::SND_CHANNEL_AMBIENT,
		Game::IW4::SND_CHANNEL_HURT,
		Game::IW4::SND_CHANNEL_PLAYER1,
		Game::IW4::SND_CHANNEL_PLAYER2,
		Game::IW4::SND_CHANNEL_MUSIC,
		Game::IW4::SND_CHANNEL_MUSICNOPAUSE,
		Game::IW4::SND_CHANNEL_MISSION,
		Game::IW4::SND_CHANNEL_ANNOUNCER,
		Game::IW4::SND_CHANNEL_SHELLSHOCK
	};

	Game::IW4::snd_alias_list_t* Isnd_alias_list_t::Convert(Game::IW3::snd_alias_list_t* ents)
	{
		if (ents->count > 32)
		{
			// This is probably garbage data
			return nullptr;
		}

		if (ents->count == 0) {
			// Very weird but it happens, notably on mp_crash_snow
			// Soundaliases with a zero-count list crash iw4 so we skip them
			// They should be empty anyway
			return nullptr;
		}

		Game::IW4::snd_alias_list_t* iw4AliasList = LocalAllocator.Allocate<Game::IW4::snd_alias_list_t>();

		iw4AliasList->aliasName = ents->aliasName;
		iw4AliasList->count = ents->count;
		iw4AliasList->head = LocalAllocator.AllocateArray<Game::IW4::snd_alias_t>(ents->count);

		for (size_t i = 0; i < ents->count; i++)
		{
			Game::IW3::snd_alias_t* iw3Alias = &ents->head[i];
			Game::IW4::snd_alias_t* iw4Alias = &iw4AliasList->head[i];

			iw4Alias->aliasName = iw3Alias->aliasName;
			iw4Alias->centerPercentage = iw3Alias->centerPercentage;
			iw4Alias->chainAliasName = iw3Alias->chainAliasName;
			iw4Alias->distMin = iw3Alias->distMin;
			iw4Alias->envelopMax = iw3Alias->envelopMax;
			iw4Alias->envelopMin = iw3Alias->envelopMin;
			iw4Alias->envelopPercentage = iw3Alias->envelopPercentage;

			// Convert flags
			Game::IW3::SoundAliasFlags iw3Flags;
			iw3Flags.intValue = iw3Alias->flags;

			Game::IW4::SoundAliasFlags iw4Flags;
			iw4Flags.intValue = 0;
			iw4Flags.looping = iw3Flags.looping;
			iw4Flags.isMaster = iw3Flags.isMaster;
			iw4Flags.isSlave = iw3Flags.isSlave;
			iw4Flags.fullDryLevel = iw3Flags.fullDryLevel;
			iw4Flags.noWetLevel = iw3Flags.noWetLevel;
			iw4Flags.type = iw3Flags.type;

			auto channel = iw3Flags.channel;

			// Channel conversion
			channel = Isnd_alias_list_t::channelLookupTable[channel];

			iw4Flags.channel = channel;

			iw4Alias->flags = iw4Flags;
			iw4Alias->lfePercentage = iw3Alias->lfePercentage;
			iw4Alias->mixerGroup = nullptr;
			iw4Alias->pitchMax = iw3Alias->pitchMax;
			iw4Alias->pitchMin = iw3Alias->pitchMin;
			iw4Alias->probability = iw3Alias->probability;

			iw4Alias->secondaryAliasName = iw3Alias->secondaryAliasName;
			iw4Alias->sequence = iw3Alias->sequence;
			iw4Alias->slavePercentage = iw3Alias->slavePercentage;
			iw4Alias->speakerMap = iw3Alias->speakerMap;
			iw4Alias->soundFile = iw3Alias->soundFile;

			iw4Alias->startDelay = iw3Alias->startDelay;
			iw4Alias->subtitle = iw3Alias->subtitle;
			iw4Alias->soundFile->type = iw3Alias->soundFile->type;

			iw4Alias->volMax = iw3Alias->volMax;
			iw4Alias->volMin = iw3Alias->volMin;
			iw4Alias->volumeFalloffCurve = LocalAllocator.Allocate<Game::IW3::SndCurve>();
			std::memcpy(iw4Alias->volumeFalloffCurve, iw3Alias->volumeFalloffCurve, sizeof(Game::IW3::SndCurve));

			if (iw4Alias->volumeFalloffCurve && iw4Alias->volumeFalloffCurve->filename)
			{
				if (iw4Alias->volumeFalloffCurve->filename == ""s)
				{
					iw4Alias->volumeFalloffCurve->filename = LocalAllocator.DuplicateString("$default.3x");
				}
			}

			if (iw4Alias->soundFile->type == Game::SAT_LOADED)
			{
				// Load sound
				iw4Alias->soundFile->u.loadSnd = AssetHandler::Convert(Game::IW3::ASSET_TYPE_LOADED_SOUND, { iw4Alias->soundFile->u.loadSnd }).loadSnd;
			}

			assert(iw4Alias->speakerMap);
		}

		return iw4AliasList;
	}

	Isnd_alias_list_t::Isnd_alias_list_t()
	{
		Command::Add("dumpSound", [](const Command::Params& params)
			{
				if (params.Length() < 2) return;
				auto converted = Isnd_alias_list_t::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_SOUND, params[1]).sound);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_SOUND, converted);
			});
	}

	Isnd_alias_list_t::~Isnd_alias_list_t()
	{
	}
}