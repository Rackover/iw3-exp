#include "STDInclude.hpp"

namespace Components
{
	std::map<Game::XAssetType, AssetHandler::AssetInterface*> AssetHandler::AssetInterfaces;
	std::vector<Game::IW3::XAsset> AssetHandler::DumpedAssets;

	void AssetHandler::Dump(Game::XAssetType type, Game::IW3::XAssetHeader header)
	{
		if(std::find_if(AssetHandler::DumpedAssets.begin(), AssetHandler::DumpedAssets.end(), [&](Game::IW3::XAsset& asset)
		{
			return (asset.type == type && asset.header.data == header.data);
		}) == AssetHandler::DumpedAssets.end())
		{
			if (AssetHandler::AssetInterfaces.find(type) != AssetHandler::AssetInterfaces.end())
			{
				AssetHandler::AssetInterfaces[type]->dump(header);
				AssetHandler::DumpedAssets.push_back({ type, header });
			}
		}
	}

	void AssetHandler::Register(AssetHandler::AssetInterface* iface)
	{
		if (iface)
		{
			Loader::Register(iface);
			AssetHandler::AssetInterfaces[iface->getType()] = iface;
		}
	}

	std::string AssetHandler::GetExportPath()
	{
		Game::dvar_s* path = Game::Dvar_FindVar("export_path");
		if (path && path->type == Game::DvarType::DVAR_TYPE_STRING && path->current.string) return path->current.string;
		return "raw_dump";
	}

	AssetHandler::AssetHandler()
	{
		AssetHandler::Register(new IXModel());
		AssetHandler::Register(new IRawFile());
		AssetHandler::Register(new IMapEnts());
		AssetHandler::Register(new IComWorld());
		AssetHandler::Register(new IGfxImage());
		AssetHandler::Register(new IGfxWorld());
		AssetHandler::Register(new IMaterial());
		AssetHandler::Register(new IclipMap_t());
		AssetHandler::Register(new IXAnimParts());
		AssetHandler::Register(new IFxEffectDef());
		AssetHandler::Register(new IGfxLightDef());
        AssetHandler::Register(new IMaterialTechniqueSet());
	}

	AssetHandler::~AssetHandler()
	{
		AssetHandler::AssetInterfaces.clear();
		AssetHandler::DumpedAssets.clear();
	}
}
