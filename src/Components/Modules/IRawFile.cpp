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


			auto entry = Game::DB_FindXAssetEntry(Game::XAssetType::ASSET_TYPE_RAWFILE, params[1]);

			if (params.Length() > 2)
			{
				int zoneIndexWhitelist = std::stoi(params[2]);
				if (entry->entry.zoneIndex == zoneIndexWhitelist)
				{
					Components::Logger::Print("Dumping rawfile %s from zone index %i\n", entry->entry.asset.header.rawfile->name, zoneIndexWhitelist);
				}
				else
				{
					Components::Logger::Print("Skipping rawfile %s from zone index %i\n", entry->entry.asset.header.rawfile->name, zoneIndexWhitelist);
					return;
				}
			}

			if (entry)
			{
				IRawFile::Dump(entry->entry.asset.header.rawfile);
			}
		});
	}

	IRawFile::~IRawFile()
	{

	}
}
