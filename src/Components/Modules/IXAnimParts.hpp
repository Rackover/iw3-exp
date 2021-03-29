#pragma once

namespace Components
{
	class IXAnimParts : public AssetHandler::AssetInterface
	{
	public:
		IXAnimParts();
		~IXAnimParts();

		const char* getName() override { return "IXAnimParts"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_XANIMPARTS; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.parts); };

	private:
		static void Dump(Game::IW3::XAnimParts* xanim);
	};
}
