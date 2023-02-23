#pragma once

namespace Components
{
	class IXModel : public AssetHandler::AssetInterface
	{

	public:
		IXModel();
		~IXModel();

		const char* GetName() override { return "IXModel"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_XMODEL; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.model) }; };

	private:
		static Game::IW4::XModel* Convert(Game::IW3::XModel* model);
	};
}
