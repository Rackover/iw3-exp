#pragma once

namespace Components
{
	class IGfxLightDef : public AssetHandler::AssetInterface
	{
	public:
		IGfxLightDef();
		~IGfxLightDef();

		const char* GetName() override { return "IGfxLightDef"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_LIGHT_DEF; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.lightDef) }; };

	private:
		static Game::IW4::GfxLightDef* Convert(Game::IW3::GfxLightDef* lightDef);
	};
}
