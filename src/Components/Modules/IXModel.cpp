#include "STDInclude.hpp"

using namespace std::literals;

static_assert(sizeof(Game::IW3::XModel) == 220, "Size of XModel is invalid!");

namespace Components
{
	Game::IW4::XModel* IXModel::Convert(Game::IW3::XModel* model)
	{
		if (!model) return nullptr;

		Game::IW4::XModel xmodel;
		ZeroMemory(&xmodel, sizeof(xmodel));

		xmodel.name = model->name;
		xmodel.numBones = model->numBones;
		xmodel.numRootBones = model->numRootBones;
		xmodel.numsurfs = model->numsurfs;
		xmodel.lodRampType = model->lodRampType;

		xmodel.scale = 1.0f;

		xmodel.boneNames = model->boneNames;
		xmodel.parentList = model->parentList;
		xmodel.quats = model->quats;
		xmodel.trans = model->trans;
		xmodel.partClassification = model->partClassification;
		xmodel.baseMat = model->baseMat;

		xmodel.materialHandles = LocalAllocator.AllocateArray<Game::IW4::Material*>(model->numsurfs);
		for (size_t i = 0; i < model->numsurfs; i++)
		{
			xmodel.materialHandles[i] = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { model->materialHandles[i]}).material;
		}

		for (int i = 0; i < 4; ++i)
		{
#if EXTEND_CULLING
			xmodel.lodInfo[i].dist = model->lodInfo[i].dist * 1.5f; // LOD distance is increased so that the maps look nicer in iw4
#else
			xmodel.lodInfo[i].dist = model->lodInfo[i].dist;
#endif
			xmodel.lodInfo[i].numsurfs = model->lodInfo[i].numsurfs;
			xmodel.lodInfo[i].surfIndex = model->lodInfo[i].surfIndex;

			// 6 vs 4 part bit elements
			std::memcpy(xmodel.lodInfo[i].partBits, model->lodInfo[i].partBits, sizeof(model->lodInfo[i].partBits));

			xmodel.lodInfo[i].lod = model->lodInfo[i].lod;

#if 0
			// Not correct
			xmodel.lodInfo[i].smcBaseIndexPlusOne = model->lodInfo[i].smcIndexPlusOne;
			xmodel.lodInfo[i].smcSubIndexMask = model->lodInfo[i].smcAllocBits;
			xmodel.lodInfo[i].smcBucket = model->lodInfo[i].unused;
#else
			xmodel.lodInfo[i].smcBaseIndexPlusOne = 0;
			xmodel.lodInfo[i].smcSubIndexMask = 0;
			xmodel.lodInfo[i].smcBucket = 0;
#endif

			if (xmodel.lodInfo[i].numsurfs)
			{
				xmodel.lodInfo[i].surfs = LocalAllocator.AllocateArray<Game::IW4::XSurface>(xmodel.lodInfo[i].numsurfs);

				for (unsigned __int16 j = 0; j < xmodel.lodInfo[i].numsurfs; ++j)
				{
					Game::IW4::XSurface* target = &xmodel.lodInfo[i].surfs[j];
					Game::IW3::XSurface* source = &model->surfs[j + xmodel.lodInfo[i].surfIndex];

					target->tileMode = source->tileMode;
					target->deformed = source->deformed;
					target->vertCount = source->vertCount;
					target->triCount = source->triCount;
					target->zoneHandle = source->zoneHandle;
					target->baseTriIndex = source->baseTriIndex;
					target->baseVertIndex = source->baseVertIndex;
					target->triIndices = source->triIndices;
					target->vertInfo = source->vertInfo;
					target->verts0 = source->verts0;
					target->vertListCount = source->vertListCount;
					target->vertList = source->vertList;

					if (i != xmodel.collLod) 
					{
						for (size_t k = 0; k < target->vertListCount; k++)
						{
							target->vertList[k].collisionTree = nullptr; // Only collod is used
						}
					}

					// 6 vs 4 part bit elements
					std::memcpy(target->partBits, source->partBits, sizeof(source->partBits));
				}

				xmodel.lodInfo[i].modelSurfs = LocalAllocator.Allocate<Game::IW4::XModelSurfs>();
				xmodel.lodInfo[i].modelSurfs->name = LocalAllocator.DuplicateString(Utils::VA("%s_lod%d", model->name, i & 0xFF));
				xmodel.lodInfo[i].modelSurfs->numSurfaces = static_cast<int>(xmodel.lodInfo[i].numsurfs);
				xmodel.lodInfo[i].modelSurfs->surfaces = xmodel.lodInfo[i].surfs;

				// 6 vs 4 part bit elements
				std::memcpy(xmodel.lodInfo[i].modelSurfs->partBits, model->lodInfo[i].partBits, sizeof(model->lodInfo[i].partBits));
			}
		}

		xmodel.numLods = static_cast<char>(model->numLods);
		xmodel.collLod = static_cast<char>(model->collLod);
		xmodel.flags = model->flags;

		if (model->collSurfs)
		{
			xmodel.collSurfs = LocalAllocator.AllocateArray<Game::IW4::XModelCollSurf_s>(model->numCollSurfs);
			for (int i = 0; i < model->numCollSurfs; ++i)
			{
				static_assert(sizeof(Game::IW3::XModelCollSurf_s) == sizeof(Game::IW4::XModelCollSurf_s), "Size mismatch");
				std::memcpy(&xmodel.collSurfs[i], &model->collSurfs[i], sizeof(Game::IW4::XModelCollSurf_s));
				xmodel.collSurfs[i].bounds.compute(model->collSurfs[i].mins, model->collSurfs[i].maxs);
			}
		}

		xmodel.numCollSurfs = model->numCollSurfs;
		xmodel.contents = model->contents;

		if (model->boneInfo)
		{
			xmodel.boneInfo = LocalAllocator.AllocateArray<Game::IW4::XBoneInfo>(model->numBones);

			for (char i = 0; i < model->numBones; ++i)
			{
				Game::IW4::XBoneInfo* target = &xmodel.boneInfo[i];
				Game::IW3::XBoneInfo* source = &model->boneInfo[i];

				target->radiusSquared = source->radiusSquared;

				target->bounds.compute(source->bounds[0], source->bounds[1]);
				target->bounds.midPoint[0] += source->offset[0];
				target->bounds.midPoint[1] += source->offset[1];
				target->bounds.midPoint[2] += source->offset[2];
			}
		}

		xmodel.radius = model->radius;

		xmodel.bounds.compute(model->mins, model->maxs);

		xmodel.memUsage = model->memUsage;
		xmodel.bad = model->bad;
		xmodel.physPreset = model->physPreset;

		if (model->physGeoms)
		{
			xmodel.physCollmap = LocalAllocator.Allocate<Game::IW4::PhysCollmap>();
			xmodel.physCollmap->name = LocalAllocator.DuplicateString(Utils::VA("%s_colmap", model->name));
			xmodel.physCollmap->count = model->physGeoms->count;
			xmodel.physCollmap->mass = model->physGeoms->mass;
			xmodel.physCollmap->bounds = xmodel.bounds; // it's fine right?
			xmodel.physCollmap->geoms = LocalAllocator.AllocateArray<Game::IW4::PhysGeomInfo>(model->physGeoms->count);

			for (unsigned int i = 0; i < model->physGeoms->count; ++i)
			{
				static_assert(sizeof(Game::IW4::PhysGeomInfo) == sizeof(Game::IW3::PhysGeomInfo), "Size mismatch");
				Game::IW4::PhysGeomInfo* target = &xmodel.physCollmap->geoms[i];
				Game::IW3::PhysGeomInfo* source = &model->physGeoms->geoms[i];

				std::memcpy(target, source, sizeof(Game::IW4::PhysGeomInfo)); // This is ok: IW3 already has "bounds" the way iw4 has, for this struct precisely

				if (source->type >= 4) 
				{
					// We're going from
					/*
						PHYS_GEOM_NONE = 0x0,
						PHYS_GEOM_BOX = 0x1,
						PHYS_GEOM_BRUSHMODEL = 0x2,
						PHYS_GEOM_BRUSH = 0x3,
						PHYS_GEOM_CYLINDER = 0x4,
						PHYS_GEOM_CAPSULE = 0x5,
							PHYS_GEOM_COUNT = 0x6,
					
					to
					
						PHYS_GEOM_NONE = 0x0,
						PHYS_GEOM_BOX = 0x1,
						PHYS_GEOM_BRUSHMODEL = 0x2,
						PHYS_GEOM_BRUSH = 0x3,
							PHYS_GEOM_COLLMAP = 0x4,
						PHYS_GEOM_CYLINDER = 0x5,
						PHYS_GEOM_CAPSULE = 0x6,
							PHYS_GEOM_GLASS = 0x7,
							PHYS_GEOM_COUNT = 0x8,
					*/

					target->type += 1; 

					Components::Logger::Print("Translated physGeomType %i into %i (geom %i from model %s)\n", source->type, target->type, i, model->name);

					if (source->type >= 0x6) 
					{
						Components::Logger::Error("Unexpected geom type %i (geom %i from model %s)\n", source->type, i, model->name);
					}
				}

				if (source->brush)
				{
					target->brush = LocalAllocator.Allocate<Game::IW4::BrushWrapper>();
					target->brush->bounds.compute(source->brush->mins, source->brush->maxs);

					target->brush->brush.numsides = static_cast<unsigned short>(source->brush->numsides);
					target->brush->brush.baseAdjacentSide = source->brush->baseAdjacentSide;
					target->brush->brush.glassPieceIndex = 0; // IW3's content is not mapped, but I doubt it belongs to this

					std::memcpy(target->brush->brush.axialMaterialNum, source->brush->axialMaterialNum, sizeof(source->brush->axialMaterialNum));
					std::memcpy(target->brush->brush.edgeCount, source->brush->edgeCount, sizeof(source->brush->edgeCount));

					for (int k = 0; k < 2; ++k)
					{
						for (int j = 0; j < 3; ++j)
						{
							target->brush->brush.firstAdjacentSideOffsets[k][j] = static_cast<char>(source->brush->firstAdjacentSideOffsets[k][j]);
						}
					}

					target->brush->planes = source->brush->planes;
					target->brush->totalEdgeCount = source->brush->totalEdgeCount;

					target->brush->brush.sides = LocalAllocator.AllocateArray<Game::IW4::cbrushside_t>(target->brush->brush.numsides);

					for (unsigned short j = 0; j < target->brush->brush.numsides; ++j)
					{
						Game::IW4::cbrushside_t* targetSide = &target->brush->brush.sides[j];
						Game::IW3::cbrushside_t* sourceSide = &source->brush->sides[j];

						targetSide->plane = sourceSide->plane;
						targetSide->materialNum = static_cast<unsigned short>(sourceSide->materialNum);
						targetSide->firstAdjacentSideOffset = static_cast<char>(sourceSide->firstAdjacentSideOffset);
						targetSide->edgeCount = sourceSide->edgeCount;
					}
				}
			}

			// Physical collision maps don't work!
			// Porting these on iw4 maps result in models with fucked up physics bouncing everywhere and making noise
			// and in some cases results in freezes if too many of them move over (fruits in carentan, papers on backlot)
			// Killing the physCollmap during porting solves this issue with no apparent drawback, surprisingly
			// The objects (car doors, fruits, ...) still work as expected when this is null. So let's keep it that way until we know what's going on!
			// The error may lie in the above codeblock, or in zonebuilder's reading of physcollmaps.
			xmodel.physCollmap = nullptr;
		}

		auto xmodelPtr = LocalAllocator.Allocate<Game::IW4::XModel>();
		*xmodelPtr = xmodel;

		return xmodelPtr;
	}

	IXModel::IXModel()
	{

		Command::Add("dumpXModel", [](const Command::Params& params)
			{
				if (params.Length() < 2) return;
				
				auto iw3Model = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_XMODEL, params[1]).model;
				auto converted = IXModel::Convert(iw3Model);
				MapDumper::GetApi()->write(Game::IW4::ASSET_TYPE_XMODEL, converted);
			});
	}

	IXModel::~IXModel()
	{

	}
}
