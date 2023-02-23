#include "STDInclude.hpp"

#define IW4X_LIGHT_VERSION "0"

namespace Components
{
	Game::IW4::GfxLightDef* IGfxLightDef::Convert(Game::IW3::GfxLightDef* asset)
	{
		if (!asset) return nullptr;

		auto result = LocalAllocator.Allocate<Game::IW4::GfxLightDef>();
		
		static_assert(sizeof(Game::IW3::GfxLightDef) == sizeof(Game::IW4::GfxLightDef));

		std::memcpy(result, asset, sizeof Game::IW4::GfxLightDef);


		if (asset->attenuation.image)
		{
			result->attenuation.image = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_IMAGE, { asset->attenuation.image }).image;
		}

		return result;
	}

	IGfxLightDef::IGfxLightDef()
	{
		Command::Add("dumpGfxLightDef", [](const Command::Params& params)
		{
			if (params.Length() < 2) return;
			auto converted = IGfxLightDef::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_LIGHT_DEF, params[1]).lightDef);
			MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_LIGHT_DEF, converted);
		});
	}

	IGfxLightDef::~IGfxLightDef()
	{

	}
}
