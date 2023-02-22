#include "STDInclude.hpp"

namespace Components
{
	Game::IW4::RawFile* IRawFile::Convert(Game::IW3::RawFile* rawfile)
	{
		if (!rawfile || !rawfile->buffer) return nullptr;

		auto iw4Rawfile = LocalAllocator.Allocate<Game::IW4::RawFile>();
		iw4Rawfile->buffer = rawfile->buffer;
		iw4Rawfile->compressedLen = 0;
		iw4Rawfile->len = rawfile->len;
		iw4Rawfile->name = rawfile->name;

		return iw4Rawfile;
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
				auto converted = IRawFile::Convert(entry->entry.asset.header.rawfile);
				MapDumper::GetApi()->write(Game::IW4::ASSET_TYPE_RAWFILE, converted);
			}
		});
	}

	IRawFile::~IRawFile()
	{

	}
}
