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
			std::string name = params[1];

			auto header = Game::DB_FindXAssetHeader(
				name.contains("mp/mp_") ? Game::IW3::XAssetType::ASSET_TYPE_GAMEWORLD_MP : Game::IW3::XAssetType::ASSET_TYPE_GAMEWORLD_SP,
				name.data()
			);

			auto converted = IGameWorld::Convert(header.gameWorldMp);
			MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_GAMEWORLD_MP, converted);
		});
	}

	IGameWorld::~IGameWorld()
	{

	}
}
