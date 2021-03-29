#include "STDInclude.hpp"

#define IW4X_LIGHT_VERSION "0"

namespace Components
{
	void IGfxLightDef::Dump(Game::IW3::GfxLightDef* asset)
	{
		if (!asset) return;
		Utils::Stream buffer;
		buffer.saveArray("IW4xLit" IW4X_LIGHT_VERSION, 8);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->attenuation.image)
		{
			buffer.saveString(asset->attenuation.image->name);
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { asset->attenuation.image });
		}

		Utils::WriteFile(Utils::VA("%s/lights/%s.iw4xLight", AssetHandler::GetExportPath().data(), asset->name), buffer.toBuffer());
	}

	IGfxLightDef::IGfxLightDef()
	{
		Command::Add("dumpGfxLightDef", [](Command::Params params)
		{
			if (params.Length() < 2) return;
			IGfxLightDef::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_LIGHT_DEF, params[1]).lightDef);
		});
	}

	IGfxLightDef::~IGfxLightDef()
	{

	}
}
