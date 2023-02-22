#include "STDInclude.hpp"

namespace Components
{
	std::vector<std::string> ILoadedSound::alreadySavedLSounds = std::vector<std::string>();
	std::map<std::string, char*> ILoadedSound::availableSoundDatas = std::map<std::string, char*>();

	void ILoadedSound::DuplicateSoundData(Game::IW3::LoadedSound* loadedSound)
	{
		char* soundCopy = new char[loadedSound->sound.info.data_len];
		ZeroMemory(soundCopy, loadedSound->sound.info.data_len);
		std::memcpy(soundCopy, &(loadedSound->sound.data), loadedSound->sound.info.data_len);

		ILoadedSound::availableSoundDatas.insert({ std::string(loadedSound->name), soundCopy });
	}

	void ILoadedSound::Dump(Game::IW3::LoadedSound* loadedSound)
	{
		if (std::find(ILoadedSound::alreadySavedLSounds.begin(), ILoadedSound::alreadySavedLSounds.end(), loadedSound->name) != ILoadedSound::alreadySavedLSounds.end())
		{
			// Already saved!
			return;
		};

		auto savedSound = ILoadedSound::availableSoundDatas.find(loadedSound->name);
		if (savedSound == ILoadedSound::availableSoundDatas.end())
		{
			Components::Logger::Error("Tried to save sound %s which was never saved before!\n", loadedSound->name);
			return;
		}

		char* soundData = ILoadedSound::availableSoundDatas[loadedSound->name];

		loadedSound->sound.data = soundData;

		MapDumper::GetApi()->write(Game::IW4::ASSET_TYPE_LOADED_SOUND, loadedSound);
	}


	ILoadedSound::ILoadedSound()
	{
	}

	ILoadedSound::~ILoadedSound()
	{
	}
}