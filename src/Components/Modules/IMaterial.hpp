#pragma once

namespace Components
{
	class IMaterial : public AssetHandler::AssetInterface
	{
	public:
		IMaterial();
		~IMaterial();

		const char* GetName() override { return "IMaterial"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_MATERIAL; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.material) }; };

	private:
		static std::unordered_map<char, char> sortKeysTable;
		static std::unordered_map<char, Game::IW4::Material> exampleMaterialForKey;

		static Game::IW4::Material* Convert(Game::IW3::Material* material);

		static char GetConvertedSortKey(Game::IW3::Material* material);

		static bool CheckSortKeyConflict(Game::IW4::Material* material, OUT Game::IW4::Material& conflictingMaterial);
	};
}
