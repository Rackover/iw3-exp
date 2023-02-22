#pragma once

namespace Components
{
	class IRawFile : public AssetHandler::AssetInterface
	{
	public:
		IRawFile();
		~IRawFile();

		const char* GetName() override { return "IRawFile"; };
		Game::XAssetType GetType() override { return Game::XAssetType::ASSET_TYPE_RAWFILE; };
		Game::IW4::XAssetHeader Convert(Game::IW3::XAssetHeader header) override { return { Convert(header.rawfile) }; };

	private:
		static Game::IW4::RawFile* Convert(Game::IW3::RawFile* rawfile);
	};
}
