#pragma once

namespace Components
{
	class IRawFile : public AssetHandler::AssetInterface
	{
	public:
		IRawFile();
		~IRawFile();

		const char* getName() override { return "IRawFile"; };
		Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_RAWFILE; };
		void dump(Game::IW3::XAssetHeader header) override { Dump(header.rawfile); };

	private:
		static void Dump(Game::IW3::RawFile* rawfile);
	};
}
