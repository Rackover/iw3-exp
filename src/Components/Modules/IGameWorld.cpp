#include "STDInclude.hpp"

#define IW4X_GAMEWORLD_VERSION 1

namespace Components
{
	void IGameWorld::Dump(Game::IW3::GameWorldMp* asset)
	{
		if (!asset) return;
		Utils::Memory::Allocator allocator;

		Game::IW4::GameWorldMp* iw4GameWorld = allocator.allocate<Game::IW4::GameWorldMp>();

		iw4GameWorld->name = asset->name;

		// Glass data generation
		iw4GameWorld->g_glassData = allocator.allocate<Game::IW4::G_GlassData>();
		ZeroMemory(iw4GameWorld->g_glassData, sizeof(Game::IW4::G_GlassData));

		SaveConvertedWorld(iw4GameWorld);
	}

	void IGameWorld::SaveConvertedWorld(Game::IW4::GameWorldMp* asset)
	{
		std::string basename(asset->name);

		Utils::EraseSubstring(basename, "maps/");
		Utils::EraseSubstring(basename, "mp/");
		Utils::EraseSubstring(basename, ".d3dbsp");

		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();
		Utils::Memory::Allocator strDuplicator;

		output.AddMember("version", IW4X_GAMEWORLD_VERSION, allocator);

		output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

		///////////////////////////////
		// None of this is necessary cause the glass data will always be empty
		// But zonebuilder doesn't know that. Let's generate structurally correct data for her
		rapidjson::Value glassData(rapidjson::kObjectType);

		rapidjson::Value pieces(rapidjson::kArrayType);
		for (size_t i = 0; i < asset->g_glassData->pieceCount; i++)
		{
			auto iwPiece = asset->g_glassData->glassPieces[i];

			rapidjson::Value piece(rapidjson::kObjectType);

			piece.AddMember("damageTaken", iwPiece.damageTaken, allocator);
			piece.AddMember("collapseTime", iwPiece.collapseTime, allocator);
			piece.AddMember("lastStateChangeTime", iwPiece.lastStateChangeTime, allocator);
			piece.AddMember("impactDir", iwPiece.impactDir, allocator);

			rapidjson::Value impactPos(rapidjson::kArrayType);
			impactPos.PushBack(iwPiece.impactPos[0], allocator);
			impactPos.PushBack(iwPiece.impactPos[1], allocator);

			piece.AddMember("impactPos", impactPos, allocator);

			pieces.PushBack(piece, allocator);
		}

		glassData.AddMember("glassPieces", pieces, allocator);
		glassData.AddMember("damageToWeaken", asset->g_glassData->damageToWeaken, allocator);
		glassData.AddMember("damageToDestroy", asset->g_glassData->damageToDestroy, allocator);

		rapidjson::Value glassNames(rapidjson::kArrayType);

		for (size_t i = 0; i < asset->g_glassData->glassNameCount; i++)
		{
			auto iwName = asset->g_glassData->glassNames[i];

			rapidjson::Value name(rapidjson::kObjectType);

			name.AddMember("nameStr", RAPIDJSON_STR(iwName.nameStr), allocator);
			name.AddMember("name", iwName.name, allocator);


			rapidjson::Value pieceIndices(rapidjson::kArrayType);

			for (size_t j = 0; j < iwName.pieceCount; j++)
			{
				pieceIndices.PushBack(iwName.pieceIndices[j], allocator);
			}

			name.AddMember("piecesIndices", pieceIndices, allocator);

			glassNames.PushBack(name, allocator);
		}

		glassData.AddMember("glassNames", glassNames, allocator);

		//
		/////////////////////////

		output.AddMember("glassData", glassData, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		const auto& dump = buff.GetString();

		Utils::WriteFile(Utils::VA("%s/gameworld/%s.iw4x.json", AssetHandler::GetExportPath().data(), basename.data()), dump);
	}

	IGameWorld::IGameWorld()
	{
		Command::Add("dumpGameWorld", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			IGameWorld::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP, params[1]).gameWorldMp);
		});
	}

	IGameWorld::~IGameWorld()
	{

	}
}
