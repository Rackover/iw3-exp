#pragma once

namespace Components
{
	class IXAnimParts : public AssetHandler::AssetInterface
	{
	public:
		IXAnimParts();
		~IXAnimParts();

		const char* GetName() override { return "IXAnimParts"; };
		Game::IW3::XAssetType GetType() override { return Game::IW3::XAssetType::ASSET_TYPE_XANIMPARTS; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.parts) }; };

	private:
		static Game::IW4::XAnimParts* Convert(Game::IW3::XAnimParts* xanim);
	};
}
