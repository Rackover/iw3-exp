#pragma once

namespace Components
{
	class AssetHandler : public Component
	{
	public:
		class AssetInterface : public Component
		{
		public:
			virtual Game::IW3::XAssetType GetType() = 0;
			virtual Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) = 0;

		protected:
			static Utils::Memory::Allocator LocalAllocator;
		};

		AssetHandler();
		~AssetHandler();
		const char* GetName() override { return "AssetHandler"; };

		static Game::IW4::XAssetHeader Convert(Game::IW3::XAssetType type, Game::IW3::XAssetHeader asset);
		static void Dump(Game::IW3::XAssetType type, Game::IW3::XAssetHeader asset);

		static std::string GetExportPath();
		static std::unordered_map<Game::IW3::XAssetType, Game::IW4::XAssetType> TypeTable;

	private:
		struct ProcessedAsset
		{
			void* originalPtr;
			Game::IW4::XAssetHeader convertedAsset;
			Game::IW3::XAssetType iw3Type;
		};

		static std::map<Game::IW3::XAssetType, AssetInterface*> AssetInterfaces;
		static std::vector<ProcessedAsset> ConvertedAssets;

		static void Register(AssetInterface* iface);
	};
}
