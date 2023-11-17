#pragma once

namespace Components
{
	class IWeapon : public AssetHandler::AssetInterface
	{
	public:
		IWeapon();
		~IWeapon();

		const char* GetName() override { return "IWeapon"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_WEAPON; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.weapon) }; };

	private:
		static Game::IW4::WeaponCompleteDef* Convert(Game::IW3::WeaponDef* comWorld);
		static Game::IW4::TracerDef* GenerateTracerDef(Game::IW3::WeaponDef* weapon);
		static void DifferentiateWeapon(Game::IW4::WeaponCompleteDef* weapon);
		static void DifferentiateModel(Game::IW4::XModel* model);
	};
}
