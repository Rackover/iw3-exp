#pragma once

namespace Components
{
	class IclipMap_t : public AssetHandler::AssetInterface
	{
	public:
		IclipMap_t();
		~IclipMap_t();

		const char* GetName() override { return "IclipMap_t"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_CLIPMAP_PVS; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.clipMap) }; };

		static Game::IW4::Bounds MakeCarePackageBounds();

	private:
		static Game::IW4::clipMap_t* Convert(const Game::IW3::clipMap_t* clipmap);
		static void AddTriggersToMap(Game::IW4::clipMap_t* iw4ClipMap);
		static void AddCarePackagesToMap(Game::IW4::clipMap_t* clipMap);
		static Game::IW4::SModelAabbNode* BuildSModelNodes(Game::IW4::clipMap_t* clipMap, unsigned int* size);
		static void OptimizeClipmap(Game::IW4::clipMap_t* clipMap);
	};
}
