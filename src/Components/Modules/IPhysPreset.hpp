#pragma once

namespace Components
{
	class IPhysPreset : public AssetHandler::AssetInterface
	{
	public:
		IPhysPreset();
		~IPhysPreset();

		const char* GetName() override { return "IPhysPreset"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_PHYSPRESET; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.physPreset) }; };

	private:
		static Game::IW4::PhysPreset* Convert(Game::IW3::PhysPreset* rawfile);
	};
}
