#pragma once

namespace Components
{
	class Isnd_alias_list_t : public AssetHandler::AssetInterface
	{
	public:
		Isnd_alias_list_t();
		~Isnd_alias_list_t();

		const char* getName() override { return "Isnd_alias_list_t"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_SOUND; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.sound); };
        static int channelLookupTable[Game::IW3::SND_CHANNEL_COUNT];

	private:
        union IW3SoundAliasFlags
        {
            struct
            {
                unsigned int looping : 1;
                unsigned int isMaster : 1;
                unsigned int isSlave : 1;
                unsigned int fullDryLevel : 1;
                unsigned int noWetLevel : 1;
                unsigned int unknown1 : 1;
                unsigned int type : 2;
                unsigned int channel : 6;
            };
            unsigned int intValue;
        };

        union IW4SoundAliasFlags
        {
            struct
            {
                unsigned int looping : 1;
                unsigned int isMaster : 1;
                unsigned int isSlave : 1;
                unsigned int fullDryLevel : 1;
                unsigned int noWetLevel : 1;
                unsigned int unknown1 : 1;
                unsigned int unknown2 : 1;
                unsigned int type : 2;
                unsigned int channel : 6;
            };
            unsigned int intValue;
        };

		static void Dump(Game::IW3::snd_alias_list_t* soundAlias);
	};
}
