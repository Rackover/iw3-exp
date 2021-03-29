#pragma once

namespace Components
{
	class IMapEnts : public AssetHandler::AssetInterface
	{
	public:
		IMapEnts();
		~IMapEnts();

		const char* getName() override { return "IMapEnts"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_MAP_ENTS; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.mapEnts); };

	private:
		static void Dump(Game::IW3::MapEnts* clipmap);
	};
}
