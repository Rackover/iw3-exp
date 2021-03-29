#include "STDInclude.hpp"

#define IW4X_ANIM_VERSION 1

namespace Components
{
	void IXAnimParts::Dump(Game::IW3::XAnimParts* xanim)
	{
		Game::IW4::XAnimParts parts;
		ZeroMemory(&parts, sizeof(parts));

		parts.name = xanim->name;
		parts.dataByteCount = xanim->dataByteCount;
		parts.dataShortCount = xanim->dataShortCount;
		parts.dataIntCount = xanim->dataIntCount;
		parts.randomDataByteCount = xanim->randomDataByteCount;
		parts.randomDataShortCount = xanim->randomDataShortCount;
		parts.randomDataIntCount = xanim->randomDataIntCount;
		parts.framecount = xanim->numframes;
		parts.notetrackCount = xanim->boneCount[10];//xanim->notifyCount;
		parts.bLoop = xanim->bLoop;
		parts.bDelta = xanim->bDelta;
		parts.assetType = xanim->assetType;
		parts.indexcount = xanim->indexCount;
		parts.framerate = xanim->framerate;
		parts.frequency = xanim->frequency;
		parts.tagnames = reinterpret_cast<short*>(xanim->names);
		parts.dataByte = xanim->dataByte;
		parts.dataShort = xanim->dataShort;
		parts.dataInt = xanim->dataInt;
		parts.randomDataByte = xanim->randomDataByte;
		parts.randomDataShort = xanim->randomDataShort;
		parts.randomDataInt = xanim->randomDataInt;
		parts.indices.data = xanim->indices.data;
		parts.notetracks = reinterpret_cast<Game::IW4::XAnimNotifyInfo*>(xanim->notify);

		std::memcpy(parts.boneCount, xanim->boneCount, 10);
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_NO_QUAT]           = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_NO_QUAT];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_HALF_QUAT]         = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_SIMPLE_QUAT];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_FULL_QUAT]         = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_NORMAL_QUAT];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_HALF_QUAT_NO_SIZE] = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_SIMPLE_QUAT_NO_SIZE];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_FULL_QUAT_NO_SIZE] = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_NORMAL_QUAT_NO_SIZE];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_SMALL_TRANS]       = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_SMALL_TRANS];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_TRANS]             = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_TRANS];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_TRANS_NO_SIZE]     = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_TRANS_NO_SIZE];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_NO_TRANS]          = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_NO_TRANS];
// 		parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_ALL]               = xanim->boneCount[Game::IW3::XAnimPartType::PART_TYPE_ALL];

		parts.bDelta = false;
		parts.delta = nullptr;

		Utils::Stream buffer;
		buffer.saveArray("IW4xAnim", 8);
		buffer.saveObject(IW4X_ANIM_VERSION);

		buffer.saveArray(&parts, 1);

		if (parts.name)
		{
			buffer.saveString(parts.name);
		}

		for (int i = 0; i < parts.boneCount[Game::IW4::XAnimPartType::PART_TYPE_ALL]; ++i)
		{
			buffer.saveString(Game::SL_ConvertToString(parts.tagnames[i]));
		}

		if (parts.notetracks)
		{
			buffer.saveArray(parts.notetracks, parts.notetrackCount);

			for (int i = 0; i < parts.notetrackCount; ++i)
			{
				buffer.saveString(Game::SL_ConvertToString(parts.notetracks[i].name));
			}
		}

		if (parts.dataByte)
		{
			buffer.saveArray(parts.dataByte, parts.dataByteCount);
		}

		if (parts.dataShort)
		{
			buffer.saveArray(parts.dataShort, parts.dataShortCount);
		}

		if (parts.dataInt)
		{
			buffer.saveArray(parts.dataInt, parts.dataIntCount);
		}

		if (parts.randomDataByte)
		{
			buffer.saveArray(parts.randomDataByte, parts.randomDataByteCount);
		}

		if (parts.randomDataShort)
		{
			buffer.saveArray(parts.randomDataShort, parts.randomDataShortCount);
		}

		if (parts.randomDataInt)
		{
			buffer.saveArray(parts.randomDataInt, parts.randomDataIntCount);
		}

		if (parts.indices.data)
		{
			if (parts.framecount < 256)
			{
				buffer.saveArray(parts.indices._1, parts.indexcount); // actually another pad
			}
			else
			{
				buffer.saveArray(parts.indices._2, parts.indexcount);
			}
		}

		Utils::WriteFile(Utils::VA("%s/xanim/%s.iw4xAnim", AssetHandler::GetExportPath().data(), parts.name), buffer.toBuffer());
	}

	IXAnimParts::IXAnimParts()
	{
		Command::Add("dumpXAnimParts", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			IXAnimParts::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_XANIMPARTS, params[1]).parts);
		});
	}

	IXAnimParts::~IXAnimParts()
	{

	}
}
