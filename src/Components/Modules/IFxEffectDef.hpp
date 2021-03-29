#pragma once

namespace Components
{
	class IFxEffectDef : public AssetHandler::AssetInterface
	{
	public:
		IFxEffectDef();
		~IFxEffectDef();

		const char* getName() override { return "IFxEffectDef"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_FX; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.fx); };

	private:
		static void Dump(Game::IW3::FxEffectDef* fx);

		static void SaveConvertedFx(Game::IW4::FxEffectDef* fx);
		static void SaveFxElemVisuals(Game::IW3::FxElemVisuals* visuals, char elemType, Utils::Stream* buffer);
	};
}
