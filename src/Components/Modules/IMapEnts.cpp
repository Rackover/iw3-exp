#include "STDInclude.hpp"

namespace Components
{
	void IMapEnts::Dump(Game::IW3::MapEnts* ents)
	{
		if (!ents) return;
		std::string basename(ents->name);

		Utils::EraseSubstring(basename, "maps/");
		Utils::EraseSubstring(basename, "mp/");
		Utils::EraseSubstring(basename, ".d3dbsp");

		std::string entString(ents->entityString, ents->numEntityChars - 1);

		Utils::Entities mapEnts(entString);
		mapEnts.deleteOldSchoolPickups(); 
		
		if (mapEnts.convertTurrets()) {
			// The map features turrets! We need to write a file somewhere to inform the converter about it so that the proper iw4 minigun source will be included
			Utils::WriteFile(Utils::VA("%s/HAS_MINIGUN", AssetHandler::GetExportPath().data()), "\0");
		}
		
		bool hasVehicles = mapEnts.convertVehicles();

		entString = mapEnts.build();

		for(auto& model : mapEnts.getModels(hasVehicles))
		{
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_XMODEL, { Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_XMODEL, model.data()).model });
		}

		Utils::WriteFile(Utils::VA("%s/mapents/%s.ents", AssetHandler::GetExportPath().data(), basename.data()), entString);
	}

	IMapEnts::IMapEnts()
	{
		Command::Add("dumpMapEnts", [](Command::Params params)
		{
			if (params.Length() < 2) return;
			IMapEnts::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_CLIPMAP_PVS, params[1]).clipMap->mapEnts);
		});
	}

	IMapEnts::~IMapEnts()
	{

	}
}