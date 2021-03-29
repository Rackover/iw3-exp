#pragma once

namespace Components
{
	class IXModel : public AssetHandler::AssetInterface
	{
	public:
		IXModel();
		~IXModel();

		const char* getName() override { return "IXModel"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_XMODEL; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.model); };

	private:
		static void Dump(Game::IW3::XModel* model);

		static void SaveConvertedModel(Game::IW4::XModel* asset);
		static void SaveXModelSurfs(Game::IW4::XModelSurfs* asset, Utils::Stream* buffer);
		static void SaveXSurface(Game::IW4::XSurface* surf, Utils::Stream* buffer);
		static void SaveXSurfaceCollisionTree(Game::IW3::XSurfaceCollisionTree* entry, Utils::Stream* buffer);
	};
}
