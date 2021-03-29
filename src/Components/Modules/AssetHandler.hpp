#pragma once

namespace Components
{
	class AssetHandler : public Component
	{
	public:
		class AssetInterface : public Component
		{
		public:
			virtual Game::XAssetType getType() = 0;
			virtual void dump(Game::IW3::XAssetHeader header) = 0;
		};

		AssetHandler();
		~AssetHandler();
		const char* getName() override { return "AssetHandler"; };

		static void Dump(Game::XAssetType type, Game::IW3::XAssetHeader asset);

		static std::string GetExportPath();

	private:
		static std::map<Game::XAssetType, AssetInterface*> AssetInterfaces;
		static std::vector<Game::IW3::XAsset> DumpedAssets;

		static void Register(AssetInterface* iface);
	};
}
