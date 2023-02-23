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
}
