#include "STDInclude.hpp"

#define IW4X_GAMEWORLD_VERSION 1

namespace Components
{
	void IGameWorld::Dump(Game::IW3::GameWorldMp* asset)
	{
		if (!asset) return;
		Utils::Memory::Allocator allocator;

		Game::IW4::GameWorldMp* iw4GameWorld = allocator.Allocate<Game::IW4::GameWorldMp>();

		iw4GameWorld->name = asset->name;

		// Glass data generation
		iw4GameWorld->g_glassData = allocator.Allocate<Game::IW4::G_GlassData>();
		ZeroMemory(iw4GameWorld->g_glassData, sizeof(Game::IW4::G_GlassData));

		SaveConvertedWorld(iw4GameWorld);
	}

	void IGameWorld::SaveConvertedWorld(Game::IW4::GameWorldMp* asset)
	{
		MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_GAMEWORLD_MP, asset);
	}

	IGameWorld::IGameWorld()
	{
		Command::Add("dumpGameWorld", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			IGameWorld::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP, params[1]).gameWorldMp);
		});
	}

	IGameWorld::~IGameWorld()
	{

	}
}
