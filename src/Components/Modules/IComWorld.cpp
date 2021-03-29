#include "STDInclude.hpp"

#define IW4X_COMMAP_VERSION 0

namespace Components
{
	void IComWorld::Dump(Game::IW3::ComWorld* asset)
	{
		if (!asset) return;
		Utils::Stream buffer;
		buffer.saveArray("IW4xComW", 8);
		buffer.saveObject(IW4X_COMMAP_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->primaryLights)
		{
			AssertSize(Game::IW3::ComPrimaryLight, 68);

			buffer.saveArray(asset->primaryLights, asset->primaryLightCount);

			for (unsigned int i = 0; i < asset->primaryLightCount; ++i)
			{
				Game::IW3::ComPrimaryLight* light = &asset->primaryLights[i];

				if (light->defName)
				{
					buffer.saveString(light->defName);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_LIGHT_DEF, { Game::DB_FindXAssetHeader(Game::ASSET_TYPE_LIGHT_DEF, light->defName).lightDef });
				}
			}
		}

		std::string basename(asset->name);

		Utils::EraseSubstring(basename, "maps/");
		Utils::EraseSubstring(basename, "mp/");
		Utils::EraseSubstring(basename, ".d3dbsp");

		Utils::WriteFile(Utils::VA("%s/comworld/%s.iw4xComWorld", AssetHandler::GetExportPath().data(), basename.data()), buffer.toBuffer());
	}

	IComWorld::IComWorld()
	{
		Command::Add("dumpComWorld", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			IComWorld::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_COMWORLD, params[1]).comWorld);
		});
	}

	IComWorld::~IComWorld()
	{

	}
}
