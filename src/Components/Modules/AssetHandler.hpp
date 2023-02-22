#pragma once

namespace Components
{
	class AssetHandler : public Component
	{
	public:
		class AssetInterface : public Component
		{
		public:
			virtual Game::XAssetType GetType() = 0;
			virtual Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) = 0;

		protected:
			static Utils::Memory::Allocator LocalAllocator;
		};

		AssetHandler();
		~AssetHandler();
		const char* GetName() override { return "AssetHandler"; };

		static Game::IW4::XAssetHeader Convert(Game::XAssetType type, Game::IW3::XAssetHeader asset);

		static std::string GetExportPath();

	private:
		static std::map<Game::XAssetType, AssetInterface*> AssetInterfaces;
		static std::vector<Game::IW3::XAsset> ConvertedAssets;

		static void Register(AssetInterface* iface);
	};
}
