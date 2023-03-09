#pragma once

namespace Components
{
	class IComWorld : public AssetHandler::AssetInterface
	{
	public:
		IComWorld();
		~IComWorld();

		const char* GetName() override { return "IComWorld"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_COMWORLD; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override;
	};
}
