#pragma once

namespace Components
{
	class ILoadedSound : public AssetHandler::AssetInterface
	{
	public:
		ILoadedSound();
		~ILoadedSound();

		const char* getName() override { return "ILoadedSound"; };
		Game::XAssetType GetType() override { return Game::XAssetType::ASSET_TYPE_LOADED_SOUND; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.loadSnd); };
		static void DuplicateSoundData(Game::IW3::LoadedSound* loadedSound);

	private:
		static std::vector<std::string> alreadySavedLSounds;
		static std::map<std::string, char*> availableSoundDatas;

		static void Dump(Game::IW3::LoadedSound* loadedSound);
	};
}
