#include "STDInclude.hpp"

namespace Components
{
	std::map<Game::XAssetType, AssetHandler::AssetInterface*> AssetHandler::AssetInterfaces;
	std::vector<Game::IW3::XAsset> AssetHandler::ConvertedAssets;

	Game::IW4::XAssetHeader AssetHandler::Convert(Game::XAssetType type, Game::IW3::XAssetHeader header)
	{
		const auto& existing = std::find_if(AssetHandler::ConvertedAssets.begin(), AssetHandler::ConvertedAssets.end(), [&](Game::IW3::XAsset& asset)
			{
				return (asset.type == type && asset.header.data == header.data);
			});
		if(existing == AssetHandler::ConvertedAssets.end())
		{
			if (AssetHandler::AssetInterfaces.contains(type))
			{
				const auto& result = AssetHandler::AssetInterfaces[type]->convert(header);
				AssetHandler::ConvertedAssets.push_back({ type, header });
				return result;
			}

			return { nullptr };
		}
		else
		{
			return { existing._Ptr->header.data };
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
		AssetHandler::Register(new Isnd_alias_list_t());
		AssetHandler::Register(new ILoadedSound());
		AssetHandler::Register(new IGameWorld());
	}

	AssetHandler::~AssetHandler()
	{
		AssetHandler::AssetInterfaces.clear();
		AssetHandler::ConvertedAssets.clear();
	}
}
