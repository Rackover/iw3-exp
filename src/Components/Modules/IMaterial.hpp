#pragma once

namespace Components
{
	class IMaterial : public AssetHandler::AssetInterface
	{
	public:
		IMaterial();
		~IMaterial();

		const char* getName() override { return "IMaterial"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_MATERIAL; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.material); };

	private:
		static std::unordered_map<char, char> sortKeysTable;
		static std::unordered_map<char, Game::IW4::Material> exampleMaterialForKey;

		static void Dump(Game::IW3::Material* material);

		static void SaveConvertedMaterial(Game::IW4::Material* asset);

		static char GetConvertedSortKey(Game::IW3::Material* material);

		static bool CheckSortKeyConflict(Game::IW4::Material* material, OUT Game::IW4::Material& conflictingMaterial);
	
		static rapidjson::Value StateBitsToJsonArray(Game::IW3::GfxStateBits* stateBits, unsigned char count, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator);
	};
}
