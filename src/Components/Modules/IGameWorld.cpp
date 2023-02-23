#include "STDInclude.hpp"

#define IW4X_GAMEWORLD_VERSION 1

namespace Components
{
	Game::IW4::GameWorldMp* IGameWorld::Convert(Game::IW3::GameWorldMp* asset)
	{
		if (!asset) return nullptr;

		Game::IW4::GameWorldMp* iw4GameWorld = LocalAllocator.Allocate<Game::IW4::GameWorldMp>();

		iw4GameWorld->name = asset->name;

		// Glass data generation
		iw4GameWorld->g_glassData = LocalAllocator.Allocate<Game::IW4::G_GlassData>();
		ZeroMemory(iw4GameWorld->g_glassData, sizeof(Game::IW4::G_GlassData));

		return iw4GameWorld;
	}

	IGameWorld::IGameWorld()
	{
		Command::Add("dumpGameWorld", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			auto converted = IGameWorld::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_GAMEWORLD_MP, params[1]).gameWorldMp);
			MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_GAMEWORLD_MP, converted);
		});
	}

	IGameWorld::~IGameWorld()
	{

	}
}
