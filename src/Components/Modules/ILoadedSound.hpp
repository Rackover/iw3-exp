#pragma once

namespace Components
{
	class ILoadedSound : public AssetHandler::AssetInterface
	{
	public:
		ILoadedSound();
		~ILoadedSound();

		const char* GetName() override { return "ILoadedSound"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_LOADED_SOUND; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override {
			Load(header.loadSnd);
			return { header.loadSnd };
		};
		static void DuplicateSoundData(Game::IW3::LoadedSound* loadedSound);

	private:
		static std::map<std::string, char*> availableSoundDatas;

		static void Load(Game::IW3::LoadedSound* loadedSound);
	};
}
