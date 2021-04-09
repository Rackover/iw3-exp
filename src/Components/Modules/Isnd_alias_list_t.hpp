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

	private:
		static void Dump(Game::IW3::snd_alias_list_t* soundAlias);
	};
}
