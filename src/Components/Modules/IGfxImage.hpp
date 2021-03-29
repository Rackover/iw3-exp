#pragma once

namespace Components
{
	class IGfxImage : public AssetHandler::AssetInterface
	{
	public:
		IGfxImage();
		~IGfxImage();

		const char* getName() override { return "IGfxImage"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_IMAGE; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.image); };

	private:
		static void Dump(Game::IW3::GfxImage* image);

		static int StoreTexture();
		static void ReleaseTexture(Game::IW3::XAssetHeader header);
	};
}
