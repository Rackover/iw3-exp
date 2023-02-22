#pragma once

namespace Components
{
	class IGfxWorld : public AssetHandler::AssetInterface
	{
	public:
		IGfxWorld();
		~IGfxWorld();

		const char* GetName() override { return "IGfxWorld"; };
		Game::XAssetType GetType() override { return Game::XAssetType::ASSET_TYPE_GFXWORLD; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.gfxWorld) }; };

		static std::unordered_set<unsigned short> removedStaticModelIndices;

	private:
		static Game::IW4::GfxWorld* Convert(Game::IW3::GfxWorld* world);

		static void RemoveModels(Game::IW4::GfxWorld* asset, const std::unordered_set<unsigned short>& indexes);
		static void RemoveIncompatibleModelsForIW4(Game::IW4::GfxWorld* asset, unsigned int method);
	};
}
