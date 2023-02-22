#pragma once

namespace Components
{
	class IGfxImage : public AssetHandler::AssetInterface
	{
	public:
		IGfxImage();
		~IGfxImage();

		const char* GetName() override { return "IGfxImage"; };
		Game::XAssetType GetType() override { return Game::XAssetType::ASSET_TYPE_IMAGE; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { Convert(header.image); };

		static std::string ConvertIWIOnTheFly(const std::string& filename);

	private:
		static Game::IW4::GfxImage* Convert(Game::IW3::GfxImage* image);

		static void CorrectSpecularImage(Game::IW3::GfxImage* image);
		static int StoreTexture();
		static void ReleaseTexture(Game::IW3::XAssetHeader header);
		static int TranslateFlags(int iw3Flags);
	};
}
