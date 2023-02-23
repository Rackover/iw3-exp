#pragma once

namespace Components
{
	class IFxEffectDef : public AssetHandler::AssetInterface
	{
	public:
		IFxEffectDef();
		~IFxEffectDef();

		const char* GetName() override { return "IFxEffectDef"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_FX; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.fx) }; };

	private:
		static Game::IW4::FxEffectDef* Convert(Game::IW3::FxEffectDef* xanim);
		static void ConvertFxElemVisuals(Game::IW3::FxElemVisuals* visuals, char iw3Type, Game::IW4::FxElemVisuals* target);
	};
}
