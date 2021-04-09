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

		Utils::Stream buffer;
		// --- RIF HEADER
		// ChunkID
		const char* chunkID = "RIFF";
		buffer.saveArray(chunkID, 4);

		// ChunkSize
		int subchunk1Size = 16;
		int subchunk2Size = loadedSound->sound.info.data_len;
		int chunkSize = 4 + (8 + subchunk1Size) + (8 + subchunk2Size);
		buffer.save(chunkSize);

		// Format
		const char* format = "WAVE";
		buffer.saveArray(format, 4);

		// --- FMT SUBCHUNK
		// Subchunk1ID
		const char* subchunk1ID = "fmt "; // Mind the space
		buffer.saveArray(subchunk1ID, 4);

		// Subchunk1Size
		buffer.save(subchunk1Size);

		// AudioFormat
		short audioFormat = static_cast<short>(loadedSound->sound.info.format);
		buffer.saveObject(audioFormat);

		// NumChannels
		short numChannels = static_cast<short>(loadedSound->sound.info.channels);
		buffer.saveObject(numChannels);

		// SampleRate
		int sampleRate = loadedSound->sound.info.rate;
		buffer.save(sampleRate);

		// ByteRate
		int byteRate = loadedSound->sound.info.rate * loadedSound->sound.info.channels * loadedSound->sound.info.bits/8;
		buffer.save(byteRate);

		// BlockAlign
		short blockAlign = static_cast<short>(loadedSound->sound.info.block_size);
		buffer.saveObject(blockAlign);

		// BitsPerSample
		short bitsPerSample = static_cast<short>(loadedSound->sound.info.bits);
		buffer.saveObject(bitsPerSample);


		// --- DATA SUBCHUNK
		const char* subchunk2ID = "data";
		buffer.saveArray(subchunk2ID, 4);

		// Data
		auto dataLength = loadedSound->sound.info.data_len;
		buffer.save(dataLength);
		buffer.save(soundData, dataLength);

		auto exportPath = AssetHandler::GetExportPath();
		auto outPath = Utils::VA("%s/loaded_sound/%s", exportPath.c_str(), loadedSound->name);

		Utils::WriteFile(outPath, buffer.toBuffer());
		Components::Logger::Print("Exported sound %s\n", loadedSound->name);
	}


	ILoadedSound::ILoadedSound()
	{
	}

	ILoadedSound::~ILoadedSound()
	{
	}
}