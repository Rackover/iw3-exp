#pragma once

namespace Components
{
	class IComWorld : public AssetHandler::AssetInterface
	{
	public:
		IComWorld();
		~IComWorld();

		const char* getName() override { return "IComWorld"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_COMWORLD; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.comWorld); };

	private:
		static void Dump(Game::IW3::ComWorld* comWorld);
	};
}
