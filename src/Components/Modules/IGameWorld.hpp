#pragma once

namespace Components
{
	class IGameWorld : public AssetHandler::AssetInterface
	{
	public:
		IGameWorld();
		~IGameWorld();

		const char* GetName() override { return "IGameWorld"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_GAMEWORLD_MP; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.gameWorldMp) }; };

	private:
		static Game::IW4::GameWorldMp* Convert(Game::IW3::GameWorldMp* comWorld);
	};
}
