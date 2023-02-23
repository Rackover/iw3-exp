#pragma once

namespace Components
{
	class Isnd_alias_list_t : public AssetHandler::AssetInterface
	{
	public:
		Isnd_alias_list_t();
		~Isnd_alias_list_t();

		const char* GetName() override { return "Isnd_alias_list_t"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_SOUND; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return {Convert(header.sound)}; };
        static int channelLookupTable[Game::IW3::SND_CHANNEL_COUNT];

	private:

		static Game::IW4::snd_alias_list_t* Convert(Game::IW3::snd_alias_list_t* soundAlias);
	};
}
