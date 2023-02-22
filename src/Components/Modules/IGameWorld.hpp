#pragma once

namespace Components
{
	class IGameWorld : public AssetHandler::AssetInterface
	{
	public:
		IGameWorld();
		~IGameWorld();

		const char* getName() override { return "IGameWorld"; };
		Game::XAssetType GetType() override { return Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.gameWorldMp); };

	private:
		static void Dump(Game::IW3::GameWorldMp* comWorld);
	    static void SaveConvertedWorld(Game::IW4::GameWorldMp* asset);
	};
}
