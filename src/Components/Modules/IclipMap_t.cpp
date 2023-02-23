#include "STDInclude.hpp"

#define IW4X_CLIPMAP_VERSION 2

namespace Components
{
	Game::IW4::SModelAabbNode* IclipMap_t::BuildSModelNodes(
		Game::IW4::clipMap_t* clipMap,
		unsigned int* size)
	{
		if (clipMap->numStaticModels - IGfxWorld::removedStaticModelIndices.size() <= 0) return nullptr;

		float maxs[3];
		float mins[3];

		maxs[0] = clipMap->staticModelList[0].absmax[0];
		maxs[1] = clipMap->staticModelList[0].absmax[1];
		maxs[2] = clipMap->staticModelList[0].absmax[2];

		mins[0] = clipMap->staticModelList[0].absmin[0];
		mins[1] = clipMap->staticModelList[0].absmin[1];
		mins[2] = clipMap->staticModelList[0].absmin[2];

		for (unsigned short i = 1; i < clipMap->numStaticModels; i++)
		{
			if (IGfxWorld::removedStaticModelIndices.contains(i))
			{
				continue;
			}

			maxs[0] = std::max(maxs[0], clipMap->staticModelList[i].absmax[0]);
			maxs[1] = std::max(maxs[1], clipMap->staticModelList[i].absmax[1]);
			maxs[2] = std::max(maxs[2], clipMap->staticModelList[i].absmax[2]);

			mins[0] = std::min(mins[0], clipMap->staticModelList[i].absmin[0]);
			mins[1] = std::min(mins[1], clipMap->staticModelList[i].absmin[1]);
			mins[2] = std::min(mins[2], clipMap->staticModelList[i].absmin[2]);
		}

		Game::IW4::SModelAabbNode* node = LocalAllocator.Allocate<Game::IW4::SModelAabbNode>();
		node->bounds.compute(mins, maxs);
		node->childCount = static_cast<short>(clipMap->numStaticModels - IGfxWorld::removedStaticModelIndices.size());
		node->firstChild = 0;

		*size = 1;
		return node;
	}

	void IclipMap_t::OptimizeClipmap(Game::IW4::clipMap_t* clipMap)
	{
		std::vector<std::pair<int, Game::IW3::cLeafBrushNode_s*>> nodesToRepair{};

		for (size_t i = 0; i < clipMap->leafbrushNodesCount; i++)
		{
			auto node = &clipMap->leafbrushNodes[i];
			if (node->leafBrushCount > 0)
			{
				bool found = false;
				for (size_t j = 0; j < clipMap->numLeafBrushes; j++)
				{
					if (&clipMap->leafbrushes[j] == node->data.leaf.brushes)
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					nodesToRepair.push_back({ i, node });
				}
			}
		}

		// Repair clipmap... i think
		for (auto kv : nodesToRepair)
		{
			auto node = kv.second;
			bool repaired = false;
			
			int repairedBrushRefs = 0;

			for (size_t j = 0; j < clipMap->numLeafBrushes; j++)
			{
				// This is an error... i think ?
				if (clipMap->leafbrushes[j] == node->data.leaf.brushes[repairedBrushRefs] && // Value correct
					&node->data.leaf.brushes[repairedBrushRefs] != &clipMap->leafbrushes[j]) // Ref incorrect
				{
					repairedBrushRefs++;

					if (repairedBrushRefs == node->leafBrushCount)
					{
						repaired = true;
						node->data.leaf.brushes = &clipMap->leafbrushes[j];
						repairedBrushRefs = 0;
						break;
					}
				}
				else
				{
					j -= repairedBrushRefs;
					repairedBrushRefs = 0;
				}
			}

			if (repaired)
			{
				Components::Logger::Print("Successfully linked node %i to brushes on clipmap\n", kv.first);
			}
		}
	}

	Game::IW4::clipMap_t* IclipMap_t::Convert(const Game::IW3::clipMap_t* clipMap)
	{
		if (!clipMap) return nullptr;

		auto iw4ClipMap = LocalAllocator.Allocate<Game::IW4::clipMap_t>();

#define COPY_MEMBER(x) iw4ClipMap->x = clipMap->x

		COPY_MEMBER(name);
		COPY_MEMBER(isInUse);
		COPY_MEMBER(planeCount);
		COPY_MEMBER(planes);

		unsigned int numStaticModels = clipMap->numStaticModels - Components::IGfxWorld::removedStaticModelIndices.size();
		iw4ClipMap->numStaticModels = numStaticModels;
		iw4ClipMap->staticModelList = LocalAllocator.AllocateArray<Game::IW4::cStaticModel_t>(numStaticModels);

		int skips = 0;
		for (unsigned short i = 0; i < clipMap->numStaticModels; ++i)
		{
			if (clipMap->staticModelList[i].xmodel)
			{
				if (Components::IGfxWorld::removedStaticModelIndices.contains(i))
				{
					skips++;
					continue;
				}

				int iw4Index = i - skips;

				std::memcpy(iw4ClipMap->staticModelList[iw4Index].absmax, clipMap->staticModelList[i].absmax, 3 * sizeof(float));
				std::memcpy(iw4ClipMap->staticModelList[iw4Index].absmin, clipMap->staticModelList[i].absmin, 3 * sizeof(float));
				std::memcpy(iw4ClipMap->staticModelList[iw4Index].origin, clipMap->staticModelList[i].origin, 3 * sizeof(float));
				std::memcpy(iw4ClipMap->staticModelList[iw4Index].invScaledAxis, clipMap->staticModelList[i].invScaledAxis, 3 * sizeof(float));
				iw4ClipMap->staticModelList[iw4Index].xmodel = AssetHandler::Convert(Game::IW3::ASSET_TYPE_XMODEL, { clipMap->staticModelList[i].xmodel }).model;
			}
			else
			{
				assert(false);
			}
		}

		COPY_MEMBER(numMaterials);
		iw4ClipMap->materials = LocalAllocator.AllocateArray<Game::IW4::dmaterial_t>(clipMap->numMaterials);
		for (size_t i = 0; i < clipMap->numMaterials; i++)
		{
			const auto iw3Material = &clipMap->materials[i];
			auto iw4Material = &iw4ClipMap->materials[i];
			iw4Material->contentFlags = iw3Material->contentFlags;
			iw4Material->surfaceFlags = iw3Material->surfaceFlags;
			iw4Material->material = iw3Material->material;
		}

		COPY_MEMBER(numBrushSides);
		iw4ClipMap->brushsides = LocalAllocator.AllocateArray<Game::IW4::cbrushside_t>(clipMap->numBrushSides);
		for (size_t i = 0; i < clipMap->numBrushSides; i++)
		{
			const auto iw3BrushSide = &clipMap->brushsides[i];
			auto iw4BrushSide = &iw4ClipMap->brushsides[i];

			iw4BrushSide->plane = iw3BrushSide->plane;
			iw4BrushSide->materialNum = static_cast<unsigned short>(iw3BrushSide->materialNum);
			iw4BrushSide->firstAdjacentSideOffset = static_cast<char>(iw3BrushSide->firstAdjacentSideOffset);
			iw4BrushSide->edgeCount = iw3BrushSide->edgeCount;
		}

		COPY_MEMBER(numBrushEdges);
		COPY_MEMBER(brushEdges);

		COPY_MEMBER(numNodes);
		COPY_MEMBER(nodes);

		COPY_MEMBER(numLeafs);
		iw4ClipMap->leafs = LocalAllocator.AllocateArray<Game::IW4::cLeaf_t>(clipMap->numLeafs);
		for (size_t i = 0; i < clipMap->numLeafs; i++)
		{
			auto iw3Leaf = &clipMap->leafs[i];
			auto iw4Leaf = &iw4ClipMap->leafs[i];

			iw4Leaf->firstCollAabbIndex = iw3Leaf->firstCollAabbIndex;
			iw4Leaf->collAabbCount = iw3Leaf->collAabbCount;
			iw4Leaf->brushContents = iw3Leaf->brushContents;
			iw4Leaf->terrainContents = iw3Leaf->terrainContents;
			Game::ConvertBounds(&iw4Leaf->bounds, iw3Leaf->mins, iw3Leaf->maxs);
			iw4Leaf->leafBrushNode = iw3Leaf->leafBrushNode;
		}

		COPY_MEMBER(leafbrushNodesCount);
		COPY_MEMBER(leafbrushNodes);

		COPY_MEMBER(numLeafBrushes);
		COPY_MEMBER(leafbrushes);

		COPY_MEMBER(numLeafSurfaces);
		COPY_MEMBER(leafsurfaces);

		COPY_MEMBER(vertCount);
		COPY_MEMBER(verts);

		COPY_MEMBER(triCount);
		COPY_MEMBER(triIndices);
		COPY_MEMBER(triEdgeIsWalkable);

		COPY_MEMBER(borderCount);
		COPY_MEMBER(borders);

		COPY_MEMBER(partitionCount);
		COPY_MEMBER(partitions);

		COPY_MEMBER(aabbTreeCount);
		iw4ClipMap->aabbTrees = LocalAllocator.AllocateArray<Game::IW4::CollisionAabbTree>(clipMap->aabbTreeCount);
		for (int i = 0; i < clipMap->aabbTreeCount; i++)
		{
			Game::IW4::CollisionAabbTree aabbTree;

			std::memcpy(aabbTree.midPoint, clipMap->aabbTrees[i].origin, sizeof(aabbTree.midPoint));
			std::memcpy(aabbTree.halfSize, clipMap->aabbTrees[i].halfSize, sizeof(aabbTree.halfSize));
			aabbTree.materialIndex = clipMap->aabbTrees[i].materialIndex;
			aabbTree.childCount = clipMap->aabbTrees[i].childCount;
			aabbTree.u = clipMap->aabbTrees[i].u;

			iw4ClipMap->aabbTrees[i] = aabbTree;
		}

		COPY_MEMBER(numSubModels);
		iw4ClipMap->cmodels = LocalAllocator.AllocateArray<Game::IW4::cmodel_t>(clipMap->numSubModels);
		for (unsigned int i = 0; i < clipMap->numSubModels; ++i)
		{
			Game::IW4::cmodel_t model;
			AssertSize(Game::IW4::cmodel_t, 68);
			std::memcpy(&model, &clipMap->cmodels[i], sizeof Game::IW4::cmodel_t);

			model.bounds.compute(clipMap->cmodels[i].mins, clipMap->cmodels[i].maxs);
			model.leaf.bounds.compute(clipMap->cmodels[i].leaf.mins, clipMap->cmodels[i].leaf.maxs);
			iw4ClipMap->cmodels[i] = model;
		}

		COPY_MEMBER(numBrushes);
		iw4ClipMap->brushes = LocalAllocator.AllocateArray<Game::IW4::cbrush_t>(clipMap->numBrushes);
		iw4ClipMap->brushBounds = LocalAllocator.AllocateArray<Game::IW4::Bounds>(clipMap->numBrushes);
		iw4ClipMap->brushContents = LocalAllocator.AllocateArray<int>(clipMap->numBrushes);

		std::unordered_map<Game::IW3::cbrush_t*, Game::IW4::cbrush_t*> brushUpgradeMap{};

		for (size_t i = 0; i < clipMap->numBrushes; i++)
		{
			const auto iw3Brush = &clipMap->brushes[i];
			const auto iw4Brush = &iw4ClipMap->brushes[i];

			brushUpgradeMap[iw3Brush] = iw4Brush;

			iw4Brush->numsides = static_cast<unsigned short>(iw3Brush->numsides);
			iw4Brush->glassPieceIndex = 0;

			iw4Brush->sides = nullptr;
			if (iw3Brush->sides)
			{
				for (size_t sideIndex = 0; sideIndex < clipMap->numBrushSides; sideIndex++)
				{
					if (iw3Brush->sides == &clipMap->brushsides[sideIndex])
					{
						iw4Brush->sides = &iw4ClipMap->brushsides[sideIndex];
						break;
					}
				}

				assert(iw4Brush->sides);
			}

			iw4Brush->baseAdjacentSide = iw3Brush->baseAdjacentSide;
			for (size_t x = 0; x < 2; x++)
			{
				for (size_t y = 0; y < 3; y++)
				{
					iw4Brush->axialMaterialNum[x][y] = iw3Brush->axialMaterialNum[x][y];
					iw4Brush->firstAdjacentSideOffsets[x][y] = static_cast<char>(iw3Brush->firstAdjacentSideOffsets[x][y]);
					iw4Brush->edgeCount[x][y] = static_cast<char>(iw3Brush->edgeCount[x][y]);
				}
			}

			iw4ClipMap->brushBounds[i].compute(iw3Brush->mins, iw3Brush->maxs);
			iw4ClipMap->brushContents[i] = iw3Brush->contents;
		}
		
		iw4ClipMap->mapEnts = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_MAP_ENTS, { clipMap->mapEnts }).mapEnts;

		iw4ClipMap->smodelNodes = IclipMap_t::BuildSModelNodes(iw4ClipMap, &iw4ClipMap->numNodes);

		std::memcpy(iw4ClipMap->dynEntCount, clipMap->dynEntCount, sizeof(unsigned short) * 2);

		for (size_t x = 0; x < 2; x++)
		{
			iw4ClipMap->dynEntDefList[x] = LocalAllocator.AllocateArray<Game::IW4::DynEntityDef>(iw4ClipMap->dynEntCount[x]);
			for (size_t i = 0; i < iw4ClipMap->dynEntCount[x]; i++)
			{
				const auto iw3Def = &clipMap->dynEntDefList[x][i];
				auto iw4Def = &iw4ClipMap->dynEntDefList[x][i];

				iw4Def->type = iw3Def->type;
				iw4Def->pose = iw3Def->pose;

				if (iw3Def->xModel)
				{
					iw4Def->xModel = AssetHandler::Convert(Game::IW3::ASSET_TYPE_XMODEL, { iw3Def->xModel }).model;
				}

				iw4Def->brushModel = iw3Def->brushModel;
				iw4Def->physicsBrushModel = iw3Def->physicsBrushModel;

				if (iw3Def->destroyFx)
				{
					iw4Def->destroyFx = AssetHandler::Convert(Game::IW3::ASSET_TYPE_FX, { iw3Def->destroyFx }).fx;
				}

				if (iw3Def->physPreset)
				{
					iw4Def->physPreset = AssetHandler::Convert(Game::IW3::ASSET_TYPE_PHYSPRESET, { iw3Def->physPreset }).physPreset;
				}

				iw4Def->health = iw3Def->health;
				iw4Def->mass = iw3Def->mass;
				iw4Def->contents = iw3Def->contents;
			}
		}

		
		COPY_MEMBER(checksum);

		OptimizeClipmap(iw4ClipMap);
		
#undef COPY_MEMBER

		AddTriggersToMap(iw4ClipMap);
		AddCarePackagesToMap(iw4ClipMap);

		return iw4ClipMap;
	}

	void IclipMap_t::AddTriggersToMap(Game::IW4::clipMap_t* iw4ClipMap)
	{
		// Add triggers
		if (iw4ClipMap->cmodels)
		{
			iw4ClipMap->mapEnts->trigger.count = iw4ClipMap->numSubModels;
			iw4ClipMap->mapEnts->trigger.hullCount = iw4ClipMap->numSubModels;

			iw4ClipMap->mapEnts->trigger.models = LocalAllocator.AllocateArray<Game::IW4::TriggerModel>(iw4ClipMap->numSubModels);
			iw4ClipMap->mapEnts->trigger.hulls = LocalAllocator.AllocateArray<Game::IW4::TriggerHull>(iw4ClipMap->numSubModels);

			std::vector<Game::IW4::TriggerSlab> slabs{};

			for (unsigned short i = 0; i < iw4ClipMap->mapEnts->trigger.count; ++i)
			{
				auto trigMod = &iw4ClipMap->mapEnts->trigger.models[i];
				auto trigHul = &iw4ClipMap->mapEnts->trigger.hulls[i];

				Game::IW4::Bounds cmodelBounds = iw4ClipMap->cmodels[i].bounds;

				trigHul->bounds = cmodelBounds;
				trigHul->contents = iw4ClipMap->cmodels[i].leaf.brushContents | iw4ClipMap->cmodels[i].leaf.terrainContents;

				trigMod->hullCount = 1;
				trigMod->firstHull = i;
				trigMod->contents = iw4ClipMap->cmodels[i].leaf.brushContents | iw4ClipMap->cmodels[i].leaf.terrainContents;

				auto* node = &iw4ClipMap->leafbrushNodes[iw4ClipMap->cmodels[i].leaf.leafBrushNode];

				if (node->leafBrushCount)
				{
					for (int j = 0; j < node->leafBrushCount; ++j)
					{
						auto brush = &iw4ClipMap->brushes[node->data.leaf.brushes[j]];

						auto baseSlab = slabs.size();
						for (unsigned int k = 0; k < brush->numsides; ++k)
						{
							Game::IW4::TriggerSlab curSlab;
							curSlab.dir[0] = brush->sides[k].plane->normal[0];
							curSlab.dir[1] = brush->sides[k].plane->normal[1];
							curSlab.dir[2] = brush->sides[k].plane->normal[2];
							curSlab.halfSize = brush->sides[k].plane->dist;
							curSlab.midPoint = 0.0f; // ??

							slabs.push_back(curSlab);
						}

						trigHul->firstSlab = static_cast<unsigned short>(baseSlab);
						trigHul->slabCount = static_cast<unsigned short>(slabs.size() - baseSlab);
					}
				}
			}

			iw4ClipMap->mapEnts->trigger.slabCount = slabs.size();
			iw4ClipMap->mapEnts->trigger.slabs = LocalAllocator.AllocateArray<Game::IW4::TriggerSlab>(slabs.size());

			for (unsigned int i = 0; i < slabs.size(); i++)
			{
				iw4ClipMap->mapEnts->trigger.slabs[i] = slabs[i];
			}
		}
	}

	void IclipMap_t::AddCarePackagesToMap(Game::IW4::clipMap_t* iw4ClipMap)
	{
		const auto brushIndex = iw4ClipMap->numBrushes;
		const auto brushNodeIndex = iw4ClipMap->leafbrushNodesCount;
		const auto brushEdgeIndex = iw4ClipMap->numBrushEdges;
		const auto materialIndex = iw4ClipMap->numMaterials;
		const auto cModelIndex = iw4ClipMap->numSubModels;

		// Materials
		constexpr auto matSize = sizeof(Game::IW4::dmaterial_t);
		auto reallocatedDMaterials = LocalAllocator.AllocateArray<Game::IW4::dmaterial_t>(iw4ClipMap->numMaterials + 1);

		memcpy_s(reallocatedDMaterials, iw4ClipMap->numMaterials * matSize, iw4ClipMap->materials, iw4ClipMap->numMaterials * matSize);

		// Add the care package material
		auto dmat = &reallocatedDMaterials[iw4ClipMap->numMaterials];
		dmat->contentFlags = 134420032;
		dmat->surfaceFlags = 13910176;
		const std::string materialName = "clip_nosight_metal";
		dmat->material = LocalAllocator.DuplicateString(materialName);

		iw4ClipMap->numMaterials++;
		iw4ClipMap->materials = reallocatedDMaterials;

		// Brush edges
		void* test = calloc(iw4ClipMap->numBrushEdges + 1, 1);

		auto reallocatedBrushEdges = LocalAllocator.AllocateArray<char>(iw4ClipMap->numBrushEdges + 1);
		memcpy_s(reallocatedBrushEdges, iw4ClipMap->numBrushEdges, iw4ClipMap->brushEdges, iw4ClipMap->numBrushEdges);
		reallocatedBrushEdges[brushEdgeIndex] = 2;

		//Redirect pointers
		unsigned int offset = reinterpret_cast<unsigned int>(reallocatedBrushEdges) - reinterpret_cast<unsigned int>(iw4ClipMap->brushEdges);
		for (size_t i = 0; i < iw4ClipMap->numBrushes; i++)
		{
			auto oldValue = *iw4ClipMap->brushes[i].baseAdjacentSide;

			iw4ClipMap->brushes[i].baseAdjacentSide = reinterpret_cast<char*>(reinterpret_cast<int>(iw4ClipMap->brushes[i].baseAdjacentSide) + offset);

			if (*iw4ClipMap->brushes[i].baseAdjacentSide != oldValue)
			{
				// Something wrong happened and i have no idea if this is normal behaviour or not
				if (*iw4ClipMap->brushes[i].edgeCount[0] == 0 && *iw4ClipMap->brushes[i].edgeCount[1] == 0)
				{
					iw4ClipMap->brushes[i].baseAdjacentSide = reallocatedBrushEdges;
				}
				else 
				{
					// Okay this is definitely wrong
					assert(false);
				}

			}
		}

		iw4ClipMap->brushEdges = reallocatedBrushEdges;
		iw4ClipMap->numBrushEdges++;

		// Leaf Brush node
		constexpr auto lbnSize = sizeof(Game::IW3::cLeafBrushNode_s);

		auto reallocatedBrushNodes = LocalAllocator.AllocateArray<Game::IW3::cLeafBrushNode_s>(iw4ClipMap->leafbrushNodesCount + 1);
		memcpy_s(reallocatedBrushNodes, iw4ClipMap->leafbrushNodesCount * lbnSize, iw4ClipMap->leafbrushNodes, iw4ClipMap->leafbrushNodesCount * lbnSize);

		// We add the care package brush node !
		Game::IW3::cLeafBrushNode_s carePackageBrushNode;
		carePackageBrushNode.axis = 0;
		carePackageBrushNode.leafBrushCount = 1;
		carePackageBrushNode.contents = 134420032;
		carePackageBrushNode.data.leaf.brushes = LocalAllocator.Allocate<unsigned short>();
		*carePackageBrushNode.data.leaf.brushes = brushIndex;

		reallocatedBrushNodes[iw4ClipMap->leafbrushNodesCount] = carePackageBrushNode;

		iw4ClipMap->leafbrushNodesCount++;
		iw4ClipMap->leafbrushNodes = reallocatedBrushNodes;


		// CModels
		constexpr auto cmodelSize = sizeof(Game::IW4::cmodel_t);

		auto reallocatedCModels = LocalAllocator.AllocateArray<Game::IW4::cmodel_t>(iw4ClipMap->numSubModels + 2);
		memcpy_s(reallocatedCModels, iw4ClipMap->numSubModels * cmodelSize, iw4ClipMap->cmodels, iw4ClipMap->numSubModels * cmodelSize);

		// We add the two care packages we added in GfxWorld
		for (size_t i = 0; i < 2; i++)
		{
			Game::IW4::cmodel_t model;
			Game::IW4::Bounds bounds = MakeCarePackageBounds();

			model.radius = 47.f;

			model.leaf.firstCollAabbIndex = static_cast<unsigned short>(iw4ClipMap->aabbTreeCount); // This is invalid, but this is how rust does it!
			model.leaf.collAabbCount = 0;
			model.leaf.brushContents = 134420032;
			model.leaf.terrainContents = 0;

			// Had to be slightly smaller
			bounds.halfSize[0] *= 0.95f;
			bounds.halfSize[1] *= 0.95f;
			bounds.halfSize[2] *= 0.95f;

			model.bounds = bounds;

			model.leaf.leafBrushNode = brushNodeIndex;
			reallocatedCModels[cModelIndex + i] = model;
		}

		iw4ClipMap->numSubModels += 2;
		iw4ClipMap->cmodels = reallocatedCModels;

		// Brushes
		constexpr auto brushSize = sizeof(Game::IW4::cbrush_t);

		auto reallocatedBrushes = LocalAllocator.AllocateArray<Game::IW4::cbrush_t>(iw4ClipMap->numBrushes + 1);
		memcpy_s(reallocatedBrushes, iw4ClipMap->numBrushes * brushSize, iw4ClipMap->brushes, iw4ClipMap->numBrushes * brushSize);

		auto bounds = MakeCarePackageBounds();
		Game::IW4::cbrush_t carePackageBrush;
		carePackageBrush.numsides = 0;
		carePackageBrush.baseAdjacentSide = &iw4ClipMap->brushEdges[brushEdgeIndex];
		carePackageBrush.sides = nullptr;
		
		for (int x = 0; x < 2; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				carePackageBrush.axialMaterialNum[x][y] = static_cast<short>(materialIndex);
			}
		}

		constexpr short adjacentSideOffset[][3] = { {0,0,0}, {4, 12, 20} };
		for (int x = 0; x < 2; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				// firstAdjacentSideOffsets
				carePackageBrush.firstAdjacentSideOffsets[x][y] = static_cast<char>(adjacentSideOffset[x][y]);
			}
		}

		constexpr char edgeCounts[] = { 4, 4, 4 };
		for (int x = 0; x < 2; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				// edgeCount
				carePackageBrush.edgeCount[x][y] = edgeCounts[y];
			}
		}
		auto reallocatedBounds = LocalAllocator.AllocateArray<Game::IW4::Bounds>(iw4ClipMap->numBrushes + 1);
		memcpy_s(reallocatedBounds, iw4ClipMap->numBrushes * sizeof(Game::IW4::Bounds), iw4ClipMap->brushBounds, iw4ClipMap->numBrushes* sizeof(Game::IW4::Bounds));

		auto reallocatedContents = LocalAllocator.AllocateArray<int>(iw4ClipMap->numBrushes + 1);
		memcpy_s(reallocatedContents, iw4ClipMap->numBrushes * sizeof(int), iw4ClipMap->brushContents, iw4ClipMap->numBrushes * sizeof(int));

		reallocatedBrushes[brushIndex] = carePackageBrush;
		reallocatedBounds[brushIndex] = bounds;
		reallocatedContents[brushIndex] = carePackageBrushNode.contents;

		iw4ClipMap->brushes = reallocatedBrushes;

		iw4ClipMap->numBrushes++;
	}

	Game::IW4::Bounds IclipMap_t::MakeCarePackageBounds()
	{
		Game::IW4::Bounds bounds;

		bounds.midPoint[0] = -1.0f;
		bounds.midPoint[1] = -0.045f;
		bounds.midPoint[2] = -0.5f;

		bounds.halfSize[0] = 30.0f;
		bounds.halfSize[1] = 17.75f;
		bounds.halfSize[2] = 16.0f;

		return bounds;
	}

	IclipMap_t::IclipMap_t()
	{
		Command::Add("dumpclipMap_t", [](const Command::Params& params)
			{
				if (params.Length() < 2) return;
				auto iw4Map = IclipMap_t::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_CLIPMAP_PVS, params[1]).clipMap);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_CLIPMAP_MP, iw4Map);
			});
	}

	IclipMap_t::~IclipMap_t()
	{

	}
}
