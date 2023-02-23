#pragma once

namespace Components
{
	class IMapEnts : public AssetHandler::AssetInterface
	{
	public:
		IMapEnts();
		~IMapEnts();

		const char* GetName() override { return "IMapEnts"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_MAP_ENTS; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.mapEnts) }; };

	private:
		static Game::IW4::MapEnts* Convert(Game::IW3::MapEnts* rawfile);
	};
}
