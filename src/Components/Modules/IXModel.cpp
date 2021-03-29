#include "STDInclude.hpp"

#define IW4X_MODEL_VERSION 5

using namespace std::literals;

static_assert(sizeof(Game::IW3::XModel) == 220, "Size of XModel is invalid!");

namespace Components
{
	void IXModel::SaveXSurfaceCollisionTree(Game::IW3::XSurfaceCollisionTree* entry, Utils::Stream* buffer)
	{
		buffer->saveObject(*entry);

		if (entry->nodes)
		{
			buffer->saveArray(entry->nodes, entry->nodeCount);
		}

		if (entry->leafs)
		{
			buffer->saveArray(entry->leafs, entry->leafCount);
		}
	}

	void IXModel::SaveXSurface(Game::IW4::XSurface* surf, Utils::Stream* buffer)
	{
		if (surf->vertInfo.vertsBlend)
		{
			buffer->saveArray(surf->vertInfo.vertsBlend, surf->vertInfo.vertCount[0] + (surf->vertInfo.vertCount[1] * 3) + (surf->vertInfo.vertCount[2] * 5) + (surf->vertInfo.vertCount[3] * 7));
		}

		// Access vertex block
		if (surf->verts0)
		{
			buffer->saveArray(surf->verts0, surf->vertCount);
		}

		// Save_XRigidVertListArray
		if (surf->vertList)
		{
			buffer->saveArray(surf->vertList, surf->vertListCount);

			for (unsigned int i = 0; i < surf->vertListCount; ++i)
			{
				Game::IW3::XRigidVertList* rigidVertList = &surf->vertList[i];

				if (rigidVertList->collisionTree)
				{
					IXModel::SaveXSurfaceCollisionTree(rigidVertList->collisionTree, buffer);
				}
			}
		}

		// Access index block
		if (surf->triIndices)
		{
			buffer->saveArray(surf->triIndices, surf->triCount * 3);
		}
	}

	void IXModel::SaveXModelSurfs(Game::IW4::XModelSurfs* asset, Utils::Stream* buffer)
	{
		buffer->saveObject(*asset);

		if (asset->name)
		{
			buffer->saveString(asset->name);
		}

		if (asset->surfaces)
		{
			buffer->saveArray(asset->surfaces, asset->numSurfaces);

			for (int i = 0; i < asset->numSurfaces; ++i)
			{
				IXModel::SaveXSurface(&asset->surfaces[i], buffer);
			}
		}
	}

	void IXModel::SaveConvertedModel(Game::IW4::XModel* asset)
	{
		Utils::Stream buffer;
		buffer.saveArray("IW4xModl", 8);
		buffer.saveObject(IW4X_MODEL_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->boneNames)
		{
			for (char i = 0; i < asset->numBones; ++i)
			{
				buffer.saveString(Game::SL_ConvertToString(asset->boneNames[i]));
			}
		}

		if (asset->parentList)
		{
			buffer.saveArray(asset->parentList, asset->numBones - asset->numRootBones);
		}

		if (asset->quats)
		{
			buffer.saveArray(asset->quats, (asset->numBones - asset->numRootBones) * 4);
		}

		if (asset->trans)
		{
			buffer.saveArray(asset->trans, (asset->numBones - asset->numRootBones) * 3);
		}

		if (asset->partClassification)
		{
			buffer.saveArray(asset->partClassification, asset->numBones);
		}

		if (asset->baseMat)
		{
			buffer.saveArray(asset->baseMat, asset->numBones);
		}

		if (asset->materialHandles)
		{
			buffer.saveArray(asset->materialHandles, asset->numsurfs);

			for (unsigned char i = 0; i < asset->numsurfs; ++i)
			{
				if (asset->materialHandles[i])
				{
					buffer.saveString(asset->materialHandles[i]->info.name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { asset->materialHandles[i] });
				}
			}
		}

		// Save_XModelLodInfoArray
		{
			for (int i = 0; i < 4; ++i)
			{
				if (asset->lodInfo[i].modelSurfs)
				{
					IXModel::SaveXModelSurfs(asset->lodInfo[i].modelSurfs, &buffer);
				}
			}
		}

		// Save_XModelCollSurfArray


		if (asset->collSurfs)
		{
			buffer.saveArray(asset->collSurfs, asset->numCollSurfs);

			for (int i = 0; i < asset->numCollSurfs; ++i)
			{
				Game::IW4::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

				if (collSurf->collTris)
				{
					buffer.saveArray(collSurf->collTris, collSurf->numCollTris);
				}
			}
		}

		if (asset->boneInfo)
		{
			buffer.saveArray(asset->boneInfo, asset->numBones);
		}

		if (asset->physPreset)
		{
			buffer.saveObject(*asset->physPreset);

			if (asset->physPreset->name)
			{
				buffer.saveString(asset->physPreset->name);
			}

			if (asset->physPreset->sndAliasPrefix)
			{
				buffer.saveString(asset->physPreset->sndAliasPrefix);
			}
		}

		if (asset->physCollmap)
		{
			Game::IW4::PhysCollmap* collmap = asset->physCollmap;
			buffer.saveObject(*collmap);

			if (collmap->name)
			{
				buffer.saveString(collmap->name);
			}

			if (collmap->geoms)
			{
				buffer.saveArray(collmap->geoms, collmap->count);

				for (unsigned int i = 0; i < collmap->count; ++i)
				{
					Game::IW4::PhysGeomInfo* geom = &collmap->geoms[i];

					if (geom->brush)
					{
						Game::IW4::BrushWrapper* brush = geom->brush;
						buffer.saveObject(*brush);
						{
							if (brush->brush.sides)
							{
								buffer.saveArray(brush->brush.sides, brush->brush.numsides);

								// Save_cbrushside_tArray
								for (unsigned short j = 0; j < brush->brush.numsides; ++j)
								{
									Game::IW4::cbrushside_t* side = &brush->brush.sides[j];

									// TODO: Add pointer support
									if (side->plane)
									{
										buffer.saveObject(*side->plane);
									}
								}
							}

							if (brush->brush.baseAdjacentSide)
							{
								buffer.saveArray(brush->brush.baseAdjacentSide, brush->totalEdgeCount);
							}
						}

						// TODO: Add pointer support
						if (brush->planes)
						{
							buffer.saveArray(brush->planes, brush->brush.numsides);
						}
					}
				}
			}
		}

		Utils::WriteFile(Utils::VA("%s/xmodel/%s.iw4xModel", AssetHandler::GetExportPath().data(), asset->name), buffer.toBuffer());
	}

	void IXModel::Dump(Game::IW3::XModel* model)
	{
		if (!model) return;

		Utils::Memory::Allocator allocator;

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
		xmodel.materialHandles = model->materialHandles;

		for (int i = 0; i < 4; ++i)
		{
			xmodel.lodInfo[i].dist = model->lodInfo[i].dist;
			xmodel.lodInfo[i].numsurfs = model->lodInfo[i].numsurfs;
			xmodel.lodInfo[i].surfIndex = model->lodInfo[i].surfIndex;

			// 6 vs 4 part bit elements
			std::memcpy(xmodel.lodInfo[i].partBits, model->lodInfo[i].partBits, sizeof(model->lodInfo[i].partBits));

			xmodel.lodInfo[i].lod = model->lodInfo[i].lod;

			// Not sure if correct
			xmodel.lodInfo[i].smcBaseIndexPlusOne = model->lodInfo[i].smcIndexPlusOne;
			xmodel.lodInfo[i].smcSubIndexMask = model->lodInfo[i].smcAllocBits;
			xmodel.lodInfo[i].smcBucket = model->lodInfo[i].unused;

			if (xmodel.lodInfo[i].numsurfs)
			{
				xmodel.lodInfo[i].surfs = allocator.allocateArray<Game::IW4::XSurface>(xmodel.lodInfo[i].numsurfs);

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

					// 6 vs 4 part bit elements
					std::memcpy(target->partBits, source->partBits, sizeof(source->partBits));
				}

				xmodel.lodInfo[i].modelSurfs = allocator.allocate<Game::IW4::XModelSurfs>();
				xmodel.lodInfo[i].modelSurfs->name = allocator.duplicateString(Utils::VA("%s_lod%d", model->name, i & 0xFF));
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
			xmodel.collSurfs = allocator.allocateArray<Game::IW4::XModelCollSurf_s>(model->numCollSurfs);
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
			xmodel.boneInfo = allocator.allocateArray<Game::IW4::XBoneInfo>(model->numBones);

			for (char i = 0; i < model->numBones; ++i)
			{
				Game::IW4::XBoneInfo* target = &xmodel.boneInfo[i];
				Game::IW3::XBoneInfo* source = &model->boneInfo[i];

				target->radiusSquared = source->radiusSquared;

				target->offset[0] = source->offset[0];
				target->offset[1] = source->offset[1];
				target->offset[2] = source->offset[2];

				// That conversion was guessed, but it actually makes sense (and it's correct!)
				target->centroid[0] = source->bounds[0][0] + ((source->bounds[1][0] + source->bounds[0][0]) / 2);
				target->centroid[1] = source->bounds[0][1] + ((source->bounds[1][1] + source->bounds[0][1]) / 2);
				target->centroid[2] = source->bounds[0][2] + ((source->bounds[1][2] + source->bounds[0][2]) / 2);
			}
		}

		xmodel.radius = model->radius;

		xmodel.bounds.compute(model->mins, model->maxs);

		xmodel.memUsage = model->memUsage;
		xmodel.bad = model->bad;
		xmodel.physPreset = model->physPreset;

		if (model->physGeoms)
		{
			xmodel.physCollmap = allocator.allocate<Game::IW4::PhysCollmap>();
			xmodel.physCollmap->name = allocator.duplicateString(Utils::VA("%s_colmap", model->name));
			xmodel.physCollmap->count = model->physGeoms->count;
			xmodel.physCollmap->mass = model->physGeoms->mass;

			xmodel.physCollmap->geoms = allocator.allocateArray<Game::IW4::PhysGeomInfo>(model->physGeoms->count);

			for(unsigned int i = 0; i < model->physGeoms->count; ++i)
			{
				static_assert(sizeof(Game::IW4::PhysGeomInfo) == sizeof(Game::IW3::PhysGeomInfo), "Size mismatch");
				Game::IW4::PhysGeomInfo* target = &xmodel.physCollmap->geoms[i];
				Game::IW3::PhysGeomInfo* source = &model->physGeoms->geoms[i];

				std::memcpy(target, source, sizeof(Game::IW4::PhysGeomInfo));

				if (source->brush)
				{
					target->brush = allocator.allocate<Game::IW4::BrushWrapper>();
					target->brush->bounds.compute(source->brush->mins, source->brush->maxs);

					target->brush->brush.numsides = static_cast<unsigned short>(source->brush->numsides);
					target->brush->brush.baseAdjacentSide = source->brush->baseAdjacentSide;
					target->brush->brush.glassPieceIndex = 0; // IW3's content is not mapped, but I doubt it belongs to this

					std::memcpy(target->brush->brush.axialMaterialNum, source->brush->axialMaterialNum, sizeof(source->brush->axialMaterialNum));
					std::memcpy(target->brush->brush.edgeCount, source->brush->edgeCount, sizeof(source->brush->edgeCount));

					for(int k = 0; k < 2; ++k)
					{
						for (int j = 0; j < 3; ++j)
						{
							target->brush->brush.firstAdjacentSideOffsets[k][j] = static_cast<char>(source->brush->firstAdjacentSideOffsets[k][j]);
						}
					}

					target->brush->planes = source->brush->planes;
					target->brush->totalEdgeCount = source->brush->totalEdgeCount;

					target->brush->brush.sides = allocator.allocateArray<Game::IW4::cbrushside_t>(target->brush->brush.numsides);

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
		}

		IXModel::SaveConvertedModel(&xmodel);
	}

	IXModel::IXModel()
	{
		Command::Add("dumpXModel", [] (Command::Params params)
		{
			if (params.Length() < 2) return;
			IXModel::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_XMODEL, params[1]).model);
		});
	}

	IXModel::~IXModel()
	{

	}
}
