#pragma once

namespace Components
{
	class IComWorld : public AssetHandler::AssetInterface
	{
	public:
		IComWorld();
		~IComWorld();

		const char* GetName() override { return "IComWorld"; };
		Game::XAssetType GetType() override { return Game::XAssetType::ASSET_TYPE_COMWORLD; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { header.comWorld }; };
	};
}
