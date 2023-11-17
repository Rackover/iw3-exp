#include "STDInclude.hpp"

namespace Components
{
	std::map<Game::IW3::XAssetType, AssetHandler::AssetInterface*> AssetHandler::AssetInterfaces{};
	std::vector<Components::AssetHandler::ProcessedAsset> AssetHandler::ConvertedAssets{};
	std::unordered_map<Game::IW3::XAssetType, Game::IW4::XAssetType> AssetHandler::TypeTable = {
		{ Game::IW3::ASSET_TYPE_PHYSPRESET, Game::IW4::ASSET_TYPE_PHYSPRESET },
		{ Game::IW3::ASSET_TYPE_XANIMPARTS, Game::IW4::ASSET_TYPE_XANIMPARTS },
		{ Game::IW3::ASSET_TYPE_XMODEL, Game::IW4::ASSET_TYPE_XMODEL },
		{ Game::IW3::ASSET_TYPE_MATERIAL, Game::IW4::ASSET_TYPE_MATERIAL },
		{ Game::IW3::ASSET_TYPE_TECHNIQUE_SET, Game::IW4::ASSET_TYPE_TECHNIQUE_SET },
		{ Game::IW3::ASSET_TYPE_IMAGE, Game::IW4::ASSET_TYPE_IMAGE },
		{ Game::IW3::ASSET_TYPE_SOUND, Game::IW4::ASSET_TYPE_SOUND },
		{ Game::IW3::ASSET_TYPE_SOUND_CURVE, Game::IW4::ASSET_TYPE_SOUND_CURVE },
		{ Game::IW3::ASSET_TYPE_LOADED_SOUND, Game::IW4::ASSET_TYPE_LOADED_SOUND },
		{ Game::IW3::ASSET_TYPE_CLIPMAP_PVS, Game::IW4::ASSET_TYPE_CLIPMAP_MP },
		{ Game::IW3::ASSET_TYPE_COMWORLD, Game::IW4::ASSET_TYPE_COMWORLD },
		{ Game::IW3::ASSET_TYPE_GAMEWORLD_SP, Game::IW4::ASSET_TYPE_GAMEWORLD_SP },
		{ Game::IW3::ASSET_TYPE_GAMEWORLD_MP, Game::IW4::ASSET_TYPE_GAMEWORLD_MP },
		{ Game::IW3::ASSET_TYPE_MAP_ENTS, Game::IW4::ASSET_TYPE_MAP_ENTS },
		{ Game::IW3::ASSET_TYPE_GFXWORLD, Game::IW4::ASSET_TYPE_GFXWORLD },
		{ Game::IW3::ASSET_TYPE_LIGHT_DEF, Game::IW4::ASSET_TYPE_LIGHT_DEF },
		{ Game::IW3::ASSET_TYPE_UI_MAP, Game::IW4::ASSET_TYPE_UI_MAP },
		{ Game::IW3::ASSET_TYPE_FONT, Game::IW4::ASSET_TYPE_FONT },
		{ Game::IW3::ASSET_TYPE_MENULIST, Game::IW4::ASSET_TYPE_MENULIST },
		{ Game::IW3::ASSET_TYPE_MENU, Game::IW4::ASSET_TYPE_MENU },
		{ Game::IW3::ASSET_TYPE_LOCALIZE_ENTRY, Game::IW4::ASSET_TYPE_LOCALIZE_ENTRY },
		{ Game::IW3::ASSET_TYPE_WEAPON, Game::IW4::ASSET_TYPE_WEAPON },
		{ Game::IW3::ASSET_TYPE_SNDDRIVER_GLOBALS, Game::IW4::ASSET_TYPE_SNDDRIVER_GLOBALS },
		{ Game::IW3::ASSET_TYPE_FX, Game::IW4::ASSET_TYPE_FX },
		{ Game::IW3::ASSET_TYPE_IMPACT_FX, Game::IW4::ASSET_TYPE_IMPACT_FX },
		{ Game::IW3::ASSET_TYPE_AITYPE, Game::IW4::ASSET_TYPE_AITYPE },
		{ Game::IW3::ASSET_TYPE_MPTYPE, Game::IW4::ASSET_TYPE_MPTYPE },
		{ Game::IW3::ASSET_TYPE_CHARACTER, Game::IW4::ASSET_TYPE_CHARACTER },
		{ Game::IW3::ASSET_TYPE_XMODELALIAS, Game::IW4::ASSET_TYPE_XMODELALIAS },
		{ Game::IW3::ASSET_TYPE_RAWFILE, Game::IW4::ASSET_TYPE_RAWFILE },
		{ Game::IW3::ASSET_TYPE_STRINGTABLE, Game::IW4::ASSET_TYPE_STRINGTABLE },
		{ Game::IW3::ASSET_TYPE_COUNT, Game::IW4::ASSET_TYPE_COUNT },
		{ Game::IW3::ASSET_TYPE_STRING, Game::IW4::ASSET_TYPE_STRING },
		{ Game::IW3::ASSET_TYPE_ASSETLIST, Game::IW4::ASSET_TYPE_ASSETLIST },

	};

	Utils::Memory::Allocator AssetHandler::AssetInterface::LocalAllocator;

	Game::IW4::XAssetHeader AssetHandler::Convert(Game::IW3::XAssetType type, Game::IW3::XAssetHeader header)
	{
		const auto& existing = std::find_if(AssetHandler::ConvertedAssets.begin(), AssetHandler::ConvertedAssets.end(), [&](ProcessedAsset& asset)
			{
				return (asset.iw3Type == type && asset.originalPtr == header.data);
			});
		if(existing == AssetHandler::ConvertedAssets.end())
		{
			if (AssetHandler::AssetInterfaces.contains(type))
			{
				const auto& result = AssetHandler::AssetInterfaces[type]->Convert(header);
				AssetHandler::ConvertedAssets.push_back({ header.data, result, type });
				return result;
			}

			return { nullptr };
		}
		else
		{
			return { existing._Ptr->convertedAsset.data };
		}
	}

	void AssetHandler::Dump(Game::IW3::XAssetType type, Game::IW3::XAssetHeader asset)
	{
		auto converted = Convert(type, asset);
		if (converted.data)
		{
			MapDumper::GetApi()->write(TypeTable[type], converted.data);
		}
	}

	void AssetHandler::Register(AssetHandler::AssetInterface* iface)
	{
		if (iface)
		{
			Loader::Register(iface);
			AssetHandler::AssetInterfaces[iface->GetType()] = iface;
		}
	}

	std::string AssetHandler::GetExportPath()
	{
		Game::dvar_s* path = Game::Dvar_FindVar("export_path");
		if (path && path->type == Game::DvarType::DVAR_TYPE_STRING && path->current.string) return path->current.string;
		return DEFAULT_WORK_DIRECTORY;
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
		AssetHandler::Register(new IWeapon());
		AssetHandler::Register(new IPhysPreset());
	}

	AssetHandler::~AssetHandler()
	{
		AssetHandler::AssetInterfaces.clear();
		AssetHandler::ConvertedAssets.clear();
	}
}
