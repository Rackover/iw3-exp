#pragma once

namespace Components
{
	class IclipMap_t : public AssetHandler::AssetInterface
	{
	public:
		IclipMap_t();
		~IclipMap_t();

		const char* getName() override { return "IclipMap_t"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_CLIPMAP_PVS; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.clipMap); };

		static Game::IW4::Bounds makeCarePackageBounds();

	private:
		static void Dump(Game::IW3::clipMap_t* clipmap);
		static Game::IW4::SModelAabbNode* BuildSModelNodes(Utils::Memory::Allocator* allocator, Game::IW3::clipMap_t* clipMap, unsigned short* size);

	};
}
