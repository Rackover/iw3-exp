#include "STDInclude.hpp"

namespace Components
{
	void IRawFile::Dump(Game::IW3::RawFile* rawfile)
	{
		if (!rawfile || !rawfile->buffer) return;
		Utils::WriteFile(Utils::VA("%s/%s", AssetHandler::GetExportPath().data(), rawfile->name), std::string(rawfile->buffer, rawfile->len));
	}

	IRawFile::IRawFile()
	{
		Command::Add("dumpRawFile", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			IRawFile::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_RAWFILE, params[1]).rawfile);
		});
	}

	IRawFile::~IRawFile()
	{

	}
}
