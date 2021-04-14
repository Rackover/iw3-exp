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

	void Isnd_alias_list_t::Dump(Game::IW3::snd_alias_list_t* ents)
	{
		if (ents->count > 32)
		{
			// This is probably garbage data
			return;
		}

		// Format is pretty transparent from iw3 to iw4, so no conversion is necessary!
		json11::Json::array head{};

		for (size_t i = 0; i < ents->count; i++)
		{
			Game::IW3::snd_alias_t iw3Alias = ents->head[i];

			json11::Json::array channelMaps{};

			for (size_t j = 0; j < 2; j++)
			{
				for (size_t k = 0; k < 2; k++)
				{
					auto iw3ChannelMap = iw3Alias.speakerMap->channelMaps[j][k];
					json11::Json::array speakers{};

					for (size_t speakerIndex = 0; speakerIndex < iw3ChannelMap.speakerCount; speakerIndex++)
					{
						auto iw3Speaker = iw3ChannelMap.speakers[speakerIndex];
						speakers.emplace_back(json11::Json::object{
							{"levels0", iw3Speaker.numLevels > 0 ? iw3Speaker.levels[0] : 0 },
							{"levels1", iw3Speaker.numLevels > 1 ? iw3Speaker.levels[1] : 0 },
							{"numLevels", iw3Speaker.numLevels },
							{"speaker", iw3Speaker.speaker }
							});
					}

					channelMaps.emplace_back(json11::Json::object{
						{"entryCount", static_cast<int>(iw3ChannelMap.speakerCount)},
						{"speakers", speakers}
						});
				}
			}

			std::string soundFile("");
			if (iw3Alias.soundFile)
			{
				switch (iw3Alias.soundFile->type)
				{
					// LOADED
				case Game::snd_alias_type_t::SAT_LOADED:
					// Save the LoadedSound subasset
					soundFile = iw3Alias.soundFile->u.loadSnd->name;
					auto header = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_LOADED_SOUND, soundFile.c_str());
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_LOADED_SOUND, header);
					break;

					// STREAMED
				case Game::snd_alias_type_t::SAT_STREAMED:
				{
					soundFile = iw3Alias.soundFile->u.streamSnd.filename.info.raw.name;

					if (iw3Alias.soundFile->u.streamSnd.filename.info.raw.dir)
					{
						soundFile = Utils::VA("%s/%s", iw3Alias.soundFile->u.streamSnd.filename.info.raw.dir, soundFile.c_str());
					}

					auto fullPath = Utils::VA("%s/sound/%s", AssetHandler::GetExportPath().c_str(), soundFile.c_str());
					auto destinationDirectory = Utils::VA("%s/sound/%s", AssetHandler::GetExportPath().c_str(), iw3Alias.soundFile->u.streamSnd.filename.info.raw.dir);
					auto internalPath = Utils::VA("sound/%s", soundFile.c_str());

					Utils::CreateDir(destinationDirectory);
					std::ofstream destination(fullPath, std::ios::binary);

					int handle;
					Game::FS_FOpenFileRead(internalPath, &handle);

					if (handle != 0)
					{
						char buffer[1024];
						int bytesRead;

						while ((bytesRead = Game::FS_Read(buffer, sizeof(buffer), handle)) > 0)
						{
							destination.write(buffer, bytesRead);
						}

						destination.flush();
						destination.close();

						Game::FS_FCloseFile(handle);
					}
					break;
				}

				// I DON'T KNOW :(
				default:
					Components::Logger::Print("Error dumping sound alias %s: unknown format %d\n", iw3Alias.aliasName, iw3Alias.soundFile->type);
					return;
				}
			}

			// Convert flags
			IW3SoundAliasFlags iw3Flags;
			iw3Flags.intValue = iw3Alias.flags;

			IW4SoundAliasFlags outputFlags;
			outputFlags.intValue = 0;
			outputFlags.looping = iw3Flags.looping;
			outputFlags.isMaster = iw3Flags.isMaster;
			outputFlags.isSlave = iw3Flags.isSlave;
			outputFlags.fullDryLevel = iw3Flags.fullDryLevel;
			outputFlags.noWetLevel = iw3Flags.noWetLevel;
			outputFlags.type = iw3Flags.type;

			auto channel = iw3Flags.channel;

			// Channel conversion
			channel = Isnd_alias_list_t::channelLookupTable[channel];

			outputFlags.channel = channel;

			auto iw4Flags = outputFlags.intValue;

			auto alias = json11::Json::object{
				{"aliasName", iw3Alias.aliasName},
				{"centerPercentage",iw3Alias.centerPercentage},
				{"chainAliasName", iw3Alias.chainAliasName == nullptr ? json11::Json() : json11::Json(std::string(iw3Alias.chainAliasName))},
				{"distMax", iw3Alias.distMax},
				{"distMin", iw3Alias.distMin},
				{"envelopMax", iw3Alias.envelopMax},
				{"envelopMin", iw3Alias.envelopMin},
				{"envelopPercentage", iw3Alias.envelopPercentage},
				{"flags", static_cast<signed int>(iw4Flags)},
				{"lfePercentage", iw3Alias.lfePercentage},
				{"mixerGroup", json11::Json()},
				{"pitchMax", iw3Alias.pitchMax},
				{"pitchMin", iw3Alias.pitchMin},
				{"probability", iw3Alias.probability},
				{"secondaryAliasName", iw3Alias.secondaryAliasName == nullptr ? json11::Json() : json11::Json(std::string(iw3Alias.secondaryAliasName))},
				{"sequence", iw3Alias.sequence},
				{"slavePercentage", iw3Alias.slavePercentage},
				{"speakerMap",  json11::Json::object{
					{"channelMaps", channelMaps},
					{"isDefault", iw3Alias.speakerMap->isDefault},
					{"name", iw3Alias.speakerMap->name}
				}},
				{"soundFile", _strdup(soundFile.c_str())},
				{"startDelay", iw3Alias.startDelay},
				{"subtitle",  iw3Alias.subtitle == nullptr ? json11::Json() : json11::Json(std::string(iw3Alias.subtitle))},
				{"type", iw3Alias.soundFile->type},
				{"volMax", iw3Alias.volMax},
				{"volMin", iw3Alias.volMin},
				{"volumeFalloffCurve", iw3Alias.volumeFalloffCurve->filename}
			};

			head.emplace_back(alias);
		}

		json11::Json aliasList = json11::Json::object{
			{"aliasName", ents->aliasName},
			{"count", static_cast<int>(ents->count)},
			{"head", head}
		};

		Utils::WriteFile(Utils::VA("%s/sounds/%s", AssetHandler::GetExportPath().data(), ents->aliasName), aliasList.dump());
	}


	Isnd_alias_list_t::Isnd_alias_list_t()
	{
		Command::Add("dumpSound", [](Command::Params params)
			{
				if (params.Length() < 2) return;
				Isnd_alias_list_t::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_SOUND, params[1]).sound);
			});
	}

	Isnd_alias_list_t::~Isnd_alias_list_t()
	{
	}
}