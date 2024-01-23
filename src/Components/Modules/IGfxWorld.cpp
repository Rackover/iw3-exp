#include "STDInclude.hpp"

#define IW4X_GFXMAP_VERSION 1

namespace Components
{
	std::unordered_set<unsigned short> IGfxWorld::removedStaticModelIndices{};

	void IGfxWorld::RemoveIncompatibleModelsForIW4(Game::IW4::GfxWorld* asset, unsigned int fixMethod)
	{
		constexpr unsigned int REMOVE_MODELS = 1;
		constexpr unsigned int SWAP_MODELS = 2;

		// Alright everybody, we're in for a ride so let's go:
		// - Static models in iw3 store all sorts of things. Vehicles, buildings, scenery
		// - Static models in iw4 don't work like that. In fact, most of them are trees, grass... nothing complex
		// IW4 has a HARD limitation (and i mean HARD) of 16 XSurfaces per LOD per static model. No more. More you crash.
		// IW3 doesn't care about this and some models have way way more. vehicle_small_hatch_green has 40 surfaces per lod.
		// The rendering in iw4 being completely differently handled, this will hard crash iw4 OR slow it down because it will
		//	try to read surfaces from other models or from litterally any garbage memory span once past the first 16 surfaces
		// This has caused numerous bugs and was hard to investigate. 
		// There are two ways to solve it:
		// - Modify iw4 rendering and make it more like iw3 rendering (easy to fuckup and frankly not a great thing to do)
		// - Move offending iw3 models to script_model in entities to make them render as Scene Entities... boom problem solved
		// We're doing that until we find another way!
		removedStaticModelIndices.clear();
		constexpr unsigned char SURF_PER_LOD_HARD_LIMIT = 16;

		for (unsigned short iw3Index = 0; iw3Index < asset->dpvs.smodelCount; iw3Index++)
		{
			auto xmodel = asset->dpvs.smodelDrawInsts[iw3Index].model;

			if (xmodel)
			{
				for (unsigned char lodIndex = 0; lodIndex < xmodel->numLods; lodIndex++)
				{
					// Oh boy
					if (xmodel->lodInfo[lodIndex].numsurfs > SURF_PER_LOD_HARD_LIMIT)
					{
						removedStaticModelIndices.insert(iw3Index);

						if (fixMethod == SWAP_MODELS)
						{
							// Poor man's fix
							if (iw3Index > 0)
							{
								auto* inst = &asset->dpvs.smodelDrawInsts[iw3Index];
								inst->model = asset->dpvs.smodelDrawInsts[iw3Index - 1].model;
								Components::Logger::Print("Swapping %s with %s model and putting its scale to zero because it is incompatible with iw4\n", xmodel->name, inst->model->name);
								inst->placement.origin[0] = std::numeric_limits<float>().min();
								inst->placement.origin[1] = std::numeric_limits<float>().min();
								inst->placement.origin[2] = std::numeric_limits<float>().min();
								inst->placement.scale = 0.f;
								removedStaticModelIndices.insert(iw3Index);

								asset->dpvs.smodelInsts[iw3Index].bounds = Game::IW4::Bounds{};
								asset->dpvs.smodelInsts[iw3Index].bounds.midPoint[0] = std::numeric_limits<float>().min();
								asset->dpvs.smodelInsts[iw3Index].bounds.midPoint[1] = std::numeric_limits<float>().min();
								asset->dpvs.smodelInsts[iw3Index].bounds.midPoint[2] = std::numeric_limits<float>().min();
							}
							else
							{
								// We could do better than this
								Components::Logger::Error("Please use another 'fix incompatible models' method. Swapping is not supported for this map.");
							}
						}
						else
						{
							Components::Logger::Print("Moving %s to entities because its model is incompatible with iw4\n", xmodel->name);
						}


					}
				}
			}
		}

		if (removedStaticModelIndices.size() > 0 && fixMethod == REMOVE_MODELS)
		{
			// Good fix, but my code to remove SModels is not perfect yet
			// It works well on some maps (mp_bloc)
			// But it breaks visdata on other maps (mp_zavod)
			// I do not know why!
			RemoveModels(asset, removedStaticModelIndices);
		}
	}

	void IGfxWorld::RemoveModels(Game::IW4::GfxWorld* asset, const std::unordered_set<unsigned short>& indices)
	{
		unsigned short previousModelCount = static_cast<unsigned short>(asset->dpvs.smodelCount);
		std::map<unsigned short, unsigned short> indexesTranslation{};

		// Update static model draw inst
		{
			unsigned short skips = 0;
			for (unsigned short iw3Index = 0; iw3Index < previousModelCount; iw3Index++)
			{
				if (indices.contains(iw3Index))
				{
					skips++;
					continue;
				}

				if (skips > 0)
				{
					asset->dpvs.smodelDrawInsts[iw3Index - skips] = asset->dpvs.smodelDrawInsts[iw3Index];
					asset->dpvs.smodelInsts[iw3Index - skips] = asset->dpvs.smodelInsts[iw3Index];
					indexesTranslation[iw3Index] = iw3Index - skips/* std::numeric_limits<unsigned short>().max()*/;
				}
				else {
					indexesTranslation[iw3Index] = iw3Index/* std::numeric_limits<unsigned short>().max()*/;
				}
			}

			asset->dpvs.smodelCount -= skips;

			if (skips == 0)
			{
				// No need to go further tbh
				return;
			}
		}

		// Update visdata
		// Might be useless because this is dynamic data...
		{
			for (int partitionIndex = 0; partitionIndex < 3; partitionIndex++)
			{
				if (asset->dpvs.smodelVisData[partitionIndex])
				{
					unsigned short skips = 0;
					for (unsigned short iw3SModelIndex = 0; iw3SModelIndex < previousModelCount; iw3SModelIndex++)
					{
						if (indices.contains(iw3SModelIndex))
						{
							skips++;
							continue;
						}

						if (skips == 0) {
							continue;
						}

#if DEBUG
						unsigned short iw4SModelIndex = iw3SModelIndex - skips;
#endif

						auto newIndex = indexesTranslation[iw3SModelIndex];

						assert(newIndex == iw4SModelIndex);

						asset->dpvs.smodelVisData[partitionIndex][newIndex] = asset->dpvs.smodelVisData[partitionIndex][iw3SModelIndex];
					}
				}
			}
		}

		// Shadow Geoms
		{
			if (asset->shadowGeom)
			{
				unsigned short skips = 0;
				for (size_t i = 0; i < asset->shadowGeom->smodelCount; i++)
				{
					if (indices.contains(asset->shadowGeom->smodelIndex[i]))
					{
						skips++;
						continue;
					}

					if (skips == 0) {
						continue;
					}

					auto oldModelIndex = asset->shadowGeom->smodelIndex[i];
					auto newModelIndex = indexesTranslation.at(oldModelIndex);

					asset->shadowGeom->smodelIndex[i - skips] = newModelIndex;
				}

				asset->shadowGeom->smodelCount -= skips;
			}
		}


		{
			// AABB Tree
			if (asset->aabbTrees)
			{
				int cellCount = asset->dpvsPlanes.cellCount;
				std::unordered_set<unsigned __int16*> convertedIndices{};

				for (int i = 0; i < cellCount; ++i)
				{
					Game::IW4::GfxCellTree* cellTree = &asset->aabbTrees[i];

					if (cellTree->aabbTree)
					{

						for (int treeIndex = 0; treeIndex < asset->aabbTreeCounts[i].aabbTreeCount; ++treeIndex)
						{
							Game::IW4::GfxAabbTree* aabbTree = &cellTree->aabbTree[treeIndex];

							if (aabbTree->smodelIndexes)
							{
								unsigned short skips = 0;
								for (unsigned short smodelIndexIndex = 0; smodelIndexIndex < aabbTree->smodelIndexCount; ++smodelIndexIndex)
								{
									if (convertedIndices.contains(&aabbTree->smodelIndexes[smodelIndexIndex]))
									{
										continue;
									}

									convertedIndices.insert(&aabbTree->smodelIndexes[smodelIndexIndex]);

									if (indices.contains(aabbTree->smodelIndexes[smodelIndexIndex]))
									{
										skips++;
										continue;
									}

									auto oldModelIndex = aabbTree->smodelIndexes[smodelIndexIndex];
									auto newModelIndex = indexesTranslation.at(oldModelIndex);

									aabbTree->smodelIndexes[smodelIndexIndex - skips] = newModelIndex;
								}

								aabbTree->smodelIndexCount -= skips;
							}
						}
					}
				}
			}
		}
	}

	Game::IW4::GfxWorld* IGfxWorld::Convert(Game::IW3::GfxWorld* world)
	{
		if (!world) return nullptr;

		Game::IW4::GfxSky* sky = LocalAllocator.Allocate<Game::IW4::GfxSky>();

		Game::IW4::GfxWorld map;
		ZeroMemory(&map, sizeof(map));

		map.name = world->name;
		map.baseName = world->baseName;
		map.planeCount = world->planeCount;
		map.nodeCount = world->nodeCount;
		map.surfaceCount = world->surfaceCount;

		map.skyCount = 1;
		map.skies = sky;

		if (world->skyImage)
		{
			sky->skyImage = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { world->skyImage }).image;
		}

		sky->skySamplerState = world->skySamplerState/* & 0xFF*/;
		sky->skyStartSurfs = world->skyStartSurfs;
		sky->skySurfCount = world->skySurfCount;

		map.lastSunPrimaryLightIndex = world->sunPrimaryLightIndex;
		map.primaryLightCount = world->primaryLightCount;

		map.dpvsPlanes = world->dpvsPlanes;




		map.dpvs.surfaceMaterials = LocalAllocator.AllocateArray<Game::IW4::GfxDrawSurf>(world->surfaceCount);
		for (auto i = 0; i < world->surfaceCount; i++)
		{
			map.dpvs.surfaceMaterials[i].fields.objectId = world->dpvs.surfaceMaterials[i].fields.objectId;
			map.dpvs.surfaceMaterials[i].fields.reflectionProbeIndex = world->dpvs.surfaceMaterials[i].fields.reflectionProbeIndex;
			map.dpvs.surfaceMaterials[i].fields.customIndex = world->dpvs.surfaceMaterials[i].fields.customIndex;
			map.dpvs.surfaceMaterials[i].fields.materialSortedIndex = world->dpvs.surfaceMaterials[i].fields.materialSortedIndex;
			map.dpvs.surfaceMaterials[i].fields.prepass = world->dpvs.surfaceMaterials[i].fields.prepass;
			map.dpvs.surfaceMaterials[i].fields.sceneLightIndex = world->dpvs.surfaceMaterials[i].fields.primaryLightIndex;
			map.dpvs.surfaceMaterials[i].fields.surfType = world->dpvs.surfaceMaterials[i].fields.surfType;
			map.dpvs.surfaceMaterials[i].fields.primarySortKey = world->dpvs.surfaceMaterials[i].fields.primarySortKey;
			map.dpvs.surfaceMaterials[i].fields.unused = world->dpvs.surfaceMaterials[i].fields.unused;
		}


		map.dpvs.surfaceCastsSunShadow = world->dpvs.surfaceCastsSunShadow;
		map.dpvs.usageCount = world->dpvs.usageCount;

		map.dpvsDyn = world->dpvsDyn;

		// iw4_credits has 0x3
		// Probably do not add Game::IW4::FogTypes::FOG_DFOG here, cod4 doesn't support it !
		map.fogTypesAllowed = Game::IW4::FogTypes::FOG_NORMAL;


		// AABBTree data is stored as part of the cells.
		// However, in IW4 it's not, so we have to extract the data
		if (world->cells)
		{
			map.aabbTreeCounts = LocalAllocator.AllocateArray<Game::IW4::GfxCellTreeCount>(world->dpvsPlanes.cellCount);
			map.aabbTrees = LocalAllocator.AllocateArray<Game::IW4::GfxCellTree>(world->dpvsPlanes.cellCount);
			map.cells = LocalAllocator.AllocateArray<Game::IW4::GfxCell>(world->dpvsPlanes.cellCount);

			for (int i = 0; i < world->dpvsPlanes.cellCount; ++i)
			{
				map.aabbTreeCounts[i].aabbTreeCount = world->cells[i].aabbTreeCount;

				map.cells[i].bounds.compute(world->cells[i].mins, world->cells[i].maxs); // Verified
				map.cells[i].portalCount = world->cells[i].portalCount;
				map.cells[i].reflectionProbeCount = world->cells[i].reflectionProbeCount;
				map.cells[i].reflectionProbes = world->cells[i].reflectionProbes;

				if (world->cells[i].aabbTree)
				{
					map.aabbTrees[i].aabbTree = LocalAllocator.AllocateArray<Game::IW4::GfxAabbTree>(world->cells[i].aabbTreeCount);
					std::memcpy(map.aabbTrees[i].aabbTree, world->cells[i].aabbTree, sizeof(Game::IW4::GfxAabbTree) * world->cells[i].aabbTreeCount);

					for (int j = 0; j < world->cells[i].aabbTreeCount; ++j)
					{
						static_assert(sizeof Game::IW4::GfxAabbTree == sizeof Game::IW3::GfxAabbTree, "Size mismatch");
						Game::IW4::GfxAabbTree* iw4Cell = &map.aabbTrees[i].aabbTree[j];
						Game::IW3::GfxAabbTree* iw3Cell = &world->cells[i].aabbTree[j];

						iw4Cell->bounds.compute(iw3Cell->mins, iw3Cell->maxs); // Verified
					}
				}

				if (world->cells[i].portals)
				{
					map.cells[i].portals = LocalAllocator.AllocateArray<Game::IW4::GfxPortal>(world->cells[i].portalCount);

					// Map all portals, so we have them ready for the next loop (might be unnecessary, as they are mapped at runtime)
					std::unordered_map<Game::IW3::GfxPortal*, Game::IW4::GfxPortal*> portalMap = { { nullptr, nullptr } };
					for (int j = 0; j < world->cells[i].portalCount; ++j)
					{
						portalMap[&world->cells[i].portals[j]] = &map.cells[i].portals[j];
					}

					for (int j = 0; j < world->cells[i].portalCount; ++j)
					{
						Game::IW3::GfxPortal* portal = &world->cells[i].portals[j];
						Game::IW4::GfxPortal* destPortal = &map.cells[i].portals[j];

						destPortal->cellIndex = static_cast<unsigned short>(portal->cell - world->cells);
						if (destPortal->cellIndex >= static_cast<unsigned short>(world->dpvsPlanes.cellCount))
						{
							Logger::Print("Unable to calculate cell index. This should not happen!\n");
							destPortal->cellIndex = 0;
						}

						destPortal->vertices = portal->vertices;
						destPortal->vertexCount = portal->vertexCount;

						destPortal->writable.isQueued = portal->writable.isQueued;
						destPortal->writable.isAncestor = portal->writable.isAncestor;
						destPortal->writable.recursionDepth = portal->writable.recursionDepth;
						destPortal->writable.hullPointCount = portal->writable.hullPointCount;
						destPortal->writable.hullPoints = portal->writable.hullPoints;

						if (portalMap.find(portal->writable.queuedParent) != portalMap.end())
						{
							destPortal->writable.queuedParent = portalMap[portal->writable.queuedParent];
						}
						else
						{
							if (portal->writable.queuedParent) Logger::Print("Unmapped portal. This shouldn't happen. Nulling it...\n");
							destPortal->writable.queuedParent = nullptr;
						}

						std::memcpy(destPortal->plane.coeffs, portal->plane.coeffs, sizeof(destPortal->plane.coeffs));
						std::memcpy(destPortal->hullAxis, portal->hullAxis, sizeof(destPortal->hullAxis));
					}
				}
			}
		}

		map.draw.reflectionProbeCount = world->reflectionProbeCount;
		map.draw.reflectionProbeTextures = LocalAllocator.AllocateArray<Game::IW4::GfxTexture>(map.draw.reflectionProbeCount);
		for (size_t i = 0; i < map.draw.reflectionProbeCount; i++)
		{
			if (world->reflectionProbeTextures[i].loadDef)
			{
				map.draw.reflectionProbeTextures[i].loadDef = IGfxImage::ConvertLoadDef(world->reflectionProbeTextures[i].loadDef);
			}
		}

		map.draw.lightmapCount = world->lightmapCount;

		map.draw.lightmaps = LocalAllocator.AllocateArray<Game::IW4::GfxLightmapArray>(map.draw.lightmapCount);
		for (int i = 0; i < map.draw.lightmapCount; i++)
		{
			auto iw4Lightmap = &map.draw.lightmaps[i];
			auto iw3Lightmap = &world->lightmaps[i];

			if (iw3Lightmap->primary)
			{
				iw4Lightmap->primary = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { iw3Lightmap->primary }).image;
			}

			if (iw3Lightmap->secondary)
			{
				iw4Lightmap->secondary = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { iw3Lightmap->secondary }).image;
			}
		}

		if (world->lightmapPrimaryTextures->data)
		{
			map.draw.lightmapPrimaryTextures = IGfxImage::ConvertTexture(world->lightmapPrimaryTextures);
		}
		else
		{
			map.draw.lightmapPrimaryTextures = LocalAllocator.Allocate<Game::IW4::GfxTexture>();
		}

		if (world->lightmapSecondaryTextures->data)
		{
			map.draw.lightmapSecondaryTextures = IGfxImage::ConvertTexture(world->lightmapSecondaryTextures);
		}
		else
		{
			map.draw.lightmapSecondaryTextures = LocalAllocator.Allocate<Game::IW4::GfxTexture>();
		}

		if (world->skyImage)
		{
			map.draw.skyImage = sky->skyImage;
		}

		if (world->outdoorImage)
		{
			map.draw.outdoorImage = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { world->outdoorImage }).image;
		}

		map.draw.vertexCount = world->vertexCount;
		map.draw.vd = world->vd;
		map.draw.vertexLayerDataSize = world->vertexLayerDataSize;
		map.draw.vld = world->vld;
		map.draw.indexCount = world->indexCount;
		map.draw.indices = world->indices;

		// Split reflection images and probes
		if (world->reflectionProbes)
		{
			map.draw.reflectionImages = LocalAllocator.AllocateArray<Game::IW4::GfxImage*>(world->reflectionProbeCount);
			map.draw.reflectionProbes = LocalAllocator.AllocateArray<Game::IW4::GfxReflectionProbe>(world->reflectionProbeCount);
			map.draw.reflectionProbeCount = world->reflectionProbeCount;

			for (unsigned int i = 0; i < world->reflectionProbeCount; ++i)
			{
				map.draw.reflectionImages[i] = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { world->reflectionProbes[i].reflectionImage }).image;

				if (world->reflectionProbes[i].reflectionImage)
				{
					assert(map.draw.reflectionImages[i]);
				}

				std::memcpy(map.draw.reflectionProbes[i].origin, world->reflectionProbes[i].origin, sizeof(map.draw.reflectionProbes[i].origin));
			}
		}

		map.lightGrid = world->lightGrid;
		map.modelCount = world->modelCount;

		static const auto addCarePackages = Game::Dvar_FindVar("iw3x_add_care_packages");
		static const auto shouldAddCarePackages = addCarePackages && addCarePackages->current.string == "1"s;
		if (world->models)
		{
			// We're about to add two brushmodels here, which are identical : one for the airdrop package and one for the 4-streak care package
			map.models = LocalAllocator.AllocateArray<Game::IW4::GfxBrushModel>(world->modelCount + (shouldAddCarePackages ? 2 : 0));

			for (int i = 0; i < world->modelCount; ++i)
			{
				map.models[i].writable.bounds.compute(world->models[i].writable.mins, world->models[i].writable.maxs); // Irrelevant, runtime data
				map.models[i].bounds.compute(world->models[i].bounds[0], world->models[i].bounds[1]); // Verified

				float* halfSize = map.models[i].bounds.halfSize;
				map.models[i].radius = static_cast<float>(std::sqrt(std::pow(halfSize[0], 2) + std::pow(halfSize[1], 2) + std::pow(halfSize[2], 2)));

				map.models[i].surfaceCount = world->models[i].surfaceCount;
				map.models[i].startSurfIndex = world->models[i].startSurfIndex;
				map.models[i].surfaceCountNoDecal = world->models[i].surfaceCountNoDecal;
			}

			if (shouldAddCarePackages)
			{
				auto index = world->modelCount;

				// Create the care packages
				Game::IW4::GfxBrushModel carePackage{};
				Game::IW4::Bounds packageBounds = IclipMap_t::MakeCarePackageBounds();

				carePackage.bounds = packageBounds;

				carePackage.radius = 47.f;
				carePackage.surfaceCount = 0;
				carePackage.surfaceCountNoDecal = 0;
				carePackage.startSurfIndex = std::numeric_limits<unsigned short>().max();

				// Airdrop package
				map.models[index++] = carePackage;

				// K4 care package
				map.models[index++] = carePackage;

				// Add 2 to modelcount because we just added two brushmodels
				map.modelCount += 2;

				// and that should be it?
			}
		}

		map.bounds.compute(world->mins, world->maxs);

		map.checksum = world->checksum;
		map.materialMemoryCount = world->materialMemoryCount;

		map.materialMemory = LocalAllocator.AllocateArray<Game::IW4::MaterialMemory>(world->materialMemoryCount);
		for (int i = 0; i < world->materialMemoryCount; i++)
		{
			map.materialMemory[i].memory = world->materialMemory[i].memory;
			map.materialMemory[i].material = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { world->materialMemory[i].material}).material;
		}
		

		static_assert(sizeof map.sun == sizeof world->sun);
		std::memcpy(&map.sun, &world->sun, sizeof map.sun);
		if (map.sun.flareMaterial)
		{
			map.sun.flareMaterial = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { world->sun.flareMaterial }).material;
		}

		if (map.sun.spriteMaterial)
		{
			map.sun.spriteMaterial = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { world->sun.spriteMaterial }).material;
		}

		std::memcpy(map.outdoorLookupMatrix, world->outdoorLookupMatrix, sizeof(map.outdoorLookupMatrix));
		map.outdoorImage = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { world->outdoorImage }).image;

		map.cellCasterBits = world->cellCasterBits;
		map.cellHasSunLitSurfsBits = reinterpret_cast<unsigned int*>(1); // This mustn't be null!

		map.sceneDynModel = world->sceneDynModel;
		map.sceneDynBrush = world->sceneDynBrush;

		map.primaryLightEntityShadowVis = world->primaryLightEntityShadowVis;
		map.primaryLightDynEntShadowVis[0] = world->primaryLightDynEntShadowVis[0];
		map.primaryLightDynEntShadowVis[1] = world->primaryLightDynEntShadowVis[1];
		map.nonSunPrimaryLightForModelDynEnt = world->nonSunPrimaryLightForModelDynEnt;

		map.shadowGeom = world->shadowGeom;
		map.lightRegion = world->lightRegion;

		map.dpvs.smodelCount = world->dpvs.smodelCount;
		map.dpvs.staticSurfaceCount = world->dpvs.staticSurfaceCount;
		map.dpvs.staticSurfaceCountNoDecal = world->dpvs.staticSurfaceCountNoDecal;

		// Not sure if correct
		// update: slightly more sure but not much lol
		map.dpvs.litOpaqueSurfsBegin = world->dpvs.litSurfsBegin;
		map.dpvs.litOpaqueSurfsEnd = world->dpvs.decalSurfsEnd;

		// these don't exist in iw3 so skip
		map.dpvs.litTransSurfsBegin = world->dpvs.decalSurfsEnd;
		map.dpvs.litTransSurfsEnd = world->dpvs.decalSurfsEnd;

		// Skip those as well
		map.dpvs.shadowCasterSurfsBegin = world->dpvs.decalSurfsEnd;
		map.dpvs.shadowCasterSurfsEnd = world->dpvs.decalSurfsEnd;

		map.dpvs.emissiveSurfsBegin = world->dpvs.emissiveSurfsBegin;
		map.dpvs.emissiveSurfsEnd = world->dpvs.emissiveSurfsEnd;
		map.dpvs.smodelVisDataCount = world->dpvs.smodelVisDataCount;
		map.dpvs.surfaceVisDataCount = world->dpvs.surfaceVisDataCount;

		std::memcpy(map.dpvs.smodelVisData, world->dpvs.smodelVisData, sizeof(map.dpvs.smodelVisData));
		std::memcpy(map.dpvs.surfaceVisData, world->dpvs.surfaceVisData, sizeof(map.dpvs.surfaceVisData));

		map.dpvs.sortedSurfIndex = world->dpvs.sortedSurfIndex;

		if (world->dpvs.smodelInsts)
		{
			map.dpvs.smodelInsts = LocalAllocator.AllocateArray<Game::IW4::GfxStaticModelInst>(world->dpvs.smodelCount);

			for (unsigned int i = 0; i < world->dpvs.smodelCount; ++i)
			{
				map.dpvs.smodelInsts[i].bounds.compute(world->dpvs.smodelInsts[i].mins, world->dpvs.smodelInsts[i].maxs); // Verified

				// this is CORRECT! Lighting origin is the place where the light grid gets sampled, and it must be the object's position!
				// This is how iw3 does it!
				// Check out 0x62EFF0 (iw3) 
				// and 0x524C80 (iw4)
				// (R_SetStaticModelLighting)
				map.dpvs.smodelInsts[i].lightingOrigin[0] = map.dpvs.smodelInsts[i].bounds.midPoint[0];
				map.dpvs.smodelInsts[i].lightingOrigin[1] = map.dpvs.smodelInsts[i].bounds.midPoint[1];
				map.dpvs.smodelInsts[i].lightingOrigin[2] = map.dpvs.smodelInsts[i].bounds.midPoint[2];
			}
		}

		if (world->dpvs.surfaces)
		{
			map.dpvs.surfaces = LocalAllocator.AllocateArray<Game::IW4::GfxSurface>(world->surfaceCount);
			map.dpvs.surfacesBounds = LocalAllocator.AllocateArray<Game::IW4::GfxSurfaceBounds>(world->surfaceCount);

			for (int i = 0; i < world->surfaceCount; ++i)
			{
				map.dpvs.surfaces[i].tris = world->dpvs.surfaces[i].tris;
				map.dpvs.surfaces[i].material = AssetHandler::Convert(Game::IW3::ASSET_TYPE_MATERIAL, { world->dpvs.surfaces[i].material }).material;

				map.dpvs.surfaces[i].lightmapIndex = world->dpvs.surfaces[i].lightmapIndex;
				map.dpvs.surfaces[i].reflectionProbeIndex = world->dpvs.surfaces[i].reflectionProbeIndex;
				map.dpvs.surfaces[i].primaryLightIndex = world->dpvs.surfaces[i].primaryLightIndex;
				map.dpvs.surfaces[i].flags = world->dpvs.surfaces[i].flags;

				map.dpvs.surfacesBounds[i].bounds.compute(world->dpvs.surfaces[i].bounds[0], world->dpvs.surfaces[i].bounds[1]); // Verified
			}
		}

		if (world->dpvs.smodelDrawInsts)
		{
			map.dpvs.smodelDrawInsts = LocalAllocator.AllocateArray<Game::IW4::GfxStaticModelDrawInst>(world->dpvs.smodelCount);

			for (unsigned int i = 0; i < world->dpvs.smodelCount; ++i)
			{
				std::memcpy(map.dpvs.smodelDrawInsts[i].placement.origin, world->dpvs.smodelDrawInsts[i].placement.origin, sizeof(map.dpvs.smodelDrawInsts[i].placement.origin));
				std::memcpy(map.dpvs.smodelDrawInsts[i].placement.axis, world->dpvs.smodelDrawInsts[i].placement.axis, sizeof(map.dpvs.smodelDrawInsts[i].placement.axis));

				std::memcpy(map.dpvs.smodelDrawInsts[i].cacheId, world->dpvs.smodelDrawInsts[i].smodelCacheIndex, sizeof(map.dpvs.smodelDrawInsts[i].cacheId));

				map.dpvs.smodelDrawInsts[i].placement.scale = world->dpvs.smodelDrawInsts[i].placement.scale;
				map.dpvs.smodelDrawInsts[i].model = AssetHandler::Convert(Game::IW3::ASSET_TYPE_XMODEL, { world->dpvs.smodelDrawInsts[i].model }).model;


				float iw3CullDist = world->dpvs.smodelDrawInsts[i].cullDist;

#if EXTEND_CULLING
				// Double cull distance so it looks nicer in iw4
				iw3CullDist *= 2;
#endif

				unsigned short iw4CullDist = 0;

				if (iw3CullDist > std::numeric_limits<unsigned short>().max())
				{
					iw4CullDist = std::numeric_limits<unsigned short>().max();
				}
				else
				{
					iw4CullDist = static_cast<unsigned short>(iw3CullDist);
				}

				map.dpvs.smodelDrawInsts[i].cullDist = iw4CullDist;

				map.dpvs.smodelDrawInsts[i].reflectionProbeIndex = world->dpvs.smodelDrawInsts[i].reflectionProbeIndex;
				map.dpvs.smodelDrawInsts[i].primaryLightIndex = world->dpvs.smodelDrawInsts[i].primaryLightIndex;
				map.dpvs.smodelDrawInsts[i].lightingHandle = world->dpvs.smodelDrawInsts[i].lightingHandle;
				map.dpvs.smodelDrawInsts[i].flags = 0;

				if (world->dpvs.smodelDrawInsts[i].flags & Game::IW3::STATIC_MODEL_FLAG_NO_SHADOW)
				{
					// Confirmed to be the same in the rendering functions
					// Check R_AddAllStaticModelSurfacesSpotShadow in both iw3 and iw4

					map.dpvs.smodelDrawInsts[i].flags |= Game::IW4::STATIC_MODEL_FLAG_NO_CAST_SHADOW;

					// aaaaand NO it's not !
					// For some reason while being used in the same place for the same thing AFAIK,
					// setting this to the "correct value" in iw4 results in blocky smodel shadows!
					// Unless we keep the iw3 flag in (which should be non existent in iw4!)
					map.dpvs.smodelDrawInsts[i].flags |= Game::IW3::STATIC_MODEL_FLAG_NO_SHADOW;
				}

				if (world->dpvs.smodelInsts)
				{
					map.dpvs.smodelDrawInsts[i].groundLighting = world->dpvs.smodelInsts[i].groundLighting;

					//// Grass needs 0x20 otherwise it doesn't read data from the lightmap and it's full bright !
					//// Whenever a model needs ground lighting in iw4, it has to specify it
					if (map.dpvs.smodelDrawInsts[i].groundLighting.packed > 0)
					{
						map.dpvs.smodelDrawInsts[i].flags |= Game::IW4::STATIC_MODEL_FLAG_GROUND_LIGHTING;
					}
				}
			}
		}

#if USE_IW3_SORTKEYS
		// IW3 values
		map.sortKeyLitDecal = 9;
		map.sortKeyEffectDecal = 29;
		map.sortKeyEffectAuto = 48;
		map.sortKeyDistortion = 0;
#else
		map.sortKeyLitDecal = 0x6;
		map.sortKeyEffectDecal = 0x27;
		map.sortKeyEffectAuto = 0x30;
		map.sortKeyDistortion = 0x2b;
#endif

		int baseIndex = 0;
		map.draw.indices = LocalAllocator.AllocateArray<unsigned short>(map.draw.indexCount);
		for (unsigned int i = 0; i < map.surfaceCount; ++i)
		{
			std::memcpy(&map.draw.indices[baseIndex], &world->indices[map.dpvs.surfaces[i].tris.baseIndex], map.dpvs.surfaces[i].tris.triCount * 6);
			map.dpvs.surfaces[i].tris.baseIndex = baseIndex;
			baseIndex += map.dpvs.surfaces[i].tris.triCount * 3;
		}

		if (baseIndex != map.draw.indexCount)
		{
			Logger::Print("Warning: Didn't sort all indicies for worldDraw");
		}

		// Specify that it's a custom map
		map.checksum = 0xC0D40001;

		auto smodelsFixMethod = Game::Dvar_FindVar("iw3x_smodels_fix_method");
		if (smodelsFixMethod)
		{
			auto method = std::stoi(smodelsFixMethod->current.string);
			IGfxWorld::RemoveIncompatibleModelsForIW4(&map, method);
		}

		auto output = LocalAllocator.Allocate<Game::IW4::GfxWorld>();
		*output = map;

		return output;
}

	IGfxWorld::IGfxWorld()
	{
		Command::Add("dumpGfxWorld", [](const Command::Params& params)
			{
				if (params.Length() < 2) return;
				const auto name = params[1];
				const auto header = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_GFXWORLD, name);
				auto converted = IGfxWorld::Convert(header.gfxWorld);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_GFXWORLD, converted);
			});
	}

	IGfxWorld::~IGfxWorld()
	{

	}
}
