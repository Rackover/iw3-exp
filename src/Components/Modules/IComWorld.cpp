#include "STDInclude.hpp"

#define IW4X_COMMAP_VERSION 0

namespace Components
{
	IComWorld::IComWorld()
	{
		Command::Add("dumpComWorld", [this] (Command::Params params)
		{
			if (params.Length() < 2) return;
			auto header = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_COMWORLD, params[1]);
			auto iw4Header = IComWorld::Convert(header);

			MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_COMWORLD, iw4Header.data);
		});
	}

	IComWorld::~IComWorld()
	{

	}
	Game::IW4::XAssetHeader IComWorld::Convert(Game::IW3::XAssetHeader header)
	{
		Game::IW3::ComWorld* iw4World = LocalAllocator.Allocate<Game::IW3::ComWorld>();

		std::memcpy(iw4World, header.comWorld, sizeof(Game::IW3::ComWorld));

		iw4World->primaryLights = LocalAllocator.AllocateArray<Game::IW3::ComPrimaryLight>(iw4World->primaryLightCount);
		for (size_t i = 0; i < header.comWorld->primaryLightCount; i++)
		{
			const auto iw3Light = &header.comWorld->primaryLights[i];
			auto iw4Light = &iw4World->primaryLights[i];

			std::memcpy(iw4Light, iw3Light, sizeof Game::IW3::ComPrimaryLight);

			if (iw3Light->defName)
			{
				iw4Light->defName = LocalAllocator.DuplicateString(iw3Light->defName);
			}

			// Gamma correction
			for (size_t channel = 0; channel < 3; channel++)
			{
				// IW4 uses 2.4F for gamma, not 2.2
				iw4Light->color[channel] = std::pow(iw3Light->color[channel], 1.f / 2.4f);
			}
		}

		return { iw4World };
	}
}
