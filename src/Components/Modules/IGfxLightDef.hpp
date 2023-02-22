#pragma once

namespace Components
{
	class IGfxLightDef : public AssetHandler::AssetInterface
	{
	public:
		IGfxLightDef();
		~IGfxLightDef();

		const char* getName() override { return "IGfxLightDef"; };
		Game::XAssetType GetType() override { return Game::XAssetType::ASSET_TYPE_LIGHT_DEF; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.lightDef); };

	private:
		static void Dump(Game::IW3::GfxLightDef* lightDef);
	};
}
