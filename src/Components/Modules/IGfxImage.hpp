#pragma once

namespace Components
{
	class IGfxImage : public AssetHandler::AssetInterface
	{
	public:
		IGfxImage();
		~IGfxImage();

		const char* GetName() override { return "IGfxImage"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_IMAGE; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.image) }; };

		static std::string ConvertIWIOnTheFly(const std::string& filename);
		static Game::IW4::GfxImageLoadDef* ConvertLoadDef(Game::IW3::GfxImageLoadDef* iw3LoadDef);
		static Game::IW4::GfxTexture* ConvertTexture(Game::IW3::GfxTexture* iw3LoadDef);

	private:
		static Game::IW4::GfxImage* Convert(Game::IW3::GfxImage* image);

		static void CorrectSpecularImage(Game::IW3::GfxImage* image);
		static int StoreTexture();
		static void ReleaseTexture(Game::IW3::XAssetHeader header);
		static int TranslateFlags(int iw3Flags);
	};
}
