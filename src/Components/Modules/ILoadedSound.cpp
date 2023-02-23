#include "STDInclude.hpp"

namespace Components
{
	std::map<std::string, char*> ILoadedSound::availableSoundDatas = std::map<std::string, char*>();

	void ILoadedSound::DuplicateSoundData(Game::IW3::LoadedSound* loadedSound)
	{
		char* soundCopy = new char[loadedSound->sound.info.data_len];
		ZeroMemory(soundCopy, loadedSound->sound.info.data_len);
		std::memcpy(soundCopy, &(loadedSound->sound.data), loadedSound->sound.info.data_len);

		ILoadedSound::availableSoundDatas.insert({ std::string(loadedSound->name), soundCopy });
	}

	void ILoadedSound::Load(Game::IW3::LoadedSound* loadedSound)
	{
		auto savedSound = ILoadedSound::availableSoundDatas.find(loadedSound->name);
		if (savedSound == ILoadedSound::availableSoundDatas.end())
		{
			Components::Logger::Error("Tried to save sound %s which was never loaded before!\n", loadedSound->name);
			return;
		}

		char* soundData = ILoadedSound::availableSoundDatas[loadedSound->name];

		loadedSound->sound.data = soundData;
	}


	ILoadedSound::ILoadedSound()
	{
	}

	ILoadedSound::~ILoadedSound()
	{
	}
}