#include "STDInclude.hpp"

#define IW4X_GFXMAP_VERSION 1

namespace Components
{
	void IGfxWorld::SaveGfxWorldDpvsStatic(Game::IW4::GfxWorld* world, Game::IW4::GfxWorldDpvsStatic* asset, Utils::Stream* buffer)
	{
		AssertSize(Game::IW4::GfxWorldDpvsStatic, 108);

		if (asset->sortedSurfIndex)
		{
			buffer->saveArray(asset->sortedSurfIndex, asset->staticSurfaceCount + asset->staticSurfaceCountNoDecal);
		}

		if (asset->smodelInsts)
		{
			AssertSize(Game::IW4::GfxStaticModelInst, 36);
			buffer->saveArray(asset->smodelInsts, asset->smodelCount);
		}

		if (asset->surfaces)
		{
			AssertSize(Game::IW4::GfxSurface, 24);
			buffer->saveArray(asset->surfaces, world->surfaceCount);

			for (unsigned int i = 0; i < world->surfaceCount; ++i)
			{
				Game::IW4::GfxSurface* surface = &asset->surfaces[i];

				if (surface->material)
				{
					buffer->saveString(world->dpvs.surfaces[i].material->info.name); // Redundant, but too lazy to implement pointer support
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { world->dpvs.surfaces[i].material });
				}
			}
		}

		if (asset->surfacesBounds)
		{
			AssertSize(Game::IW4::GfxSurfaceBounds, 24);
			buffer->saveArray(asset->surfacesBounds, world->surfaceCount);
		}

		if (asset->smodelDrawInsts)
		{
			AssertSize(Game::IW4::GfxStaticModelDrawInst, 76);
			buffer->saveArray(asset->smodelDrawInsts, asset->smodelCount);

			for (unsigned int i = 0; i < asset->smodelCount; ++i)
			{
				Game::IW4::GfxStaticModelDrawInst* model = &asset->smodelDrawInsts[i];

				if (model->model)
				{
					buffer->saveString(model->model->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_XMODEL, { model->model });
				}
			}
		}
	}

	void IGfxWorld::SaveGfxLightGrid(Game::IW3::GfxLightGrid* asset, Utils::Stream* buffer)
	{
		AssertSize(Game::IW3::GfxLightGrid, 56);

		if (asset->rowDataStart)
		{
			buffer->saveArray(asset->rowDataStart, (asset->maxs[asset->rowAxis] - asset->mins[asset->rowAxis]) + 1);
		}

		if (asset->rawRowData)
		{
			// no align for char
			buffer->saveArray(asset->rawRowData, asset->rawRowDataSize);
		}

		if (asset->entries)
		{
			AssertSize(Game::IW3::GfxLightGridEntry, 4);
			buffer->saveArray(asset->entries, asset->entryCount);
		}

		if (asset->colors)
		{
			AssertSize(Game::IW3::GfxLightGridColors, 168);
			buffer->saveArray(asset->colors, asset->colorCount);
		}
	}

	void IGfxWorld::SaveGfxWorldDraw(Game::IW4::GfxWorldDraw* asset, Utils::Stream* buffer)
	{
		AssertSize(Game::IW4::GfxWorldDraw, 72);

		if (asset->reflectionImages)
		{
			buffer->saveArray(asset->reflectionImages, asset->reflectionProbeCount);

			for (unsigned int i = 0; i < asset->reflectionProbeCount; ++i)
			{
				if (asset->reflectionImages[i])
				{
					buffer->saveString(asset->reflectionImages[i]->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { asset->reflectionImages[i] });
				}
			}
		}

		if (asset->reflectionProbes)
		{
			AssertSize(Game::IW4::GfxReflectionProbe, 12);
			buffer->saveArray(asset->reflectionProbes, asset->reflectionProbeCount);
		}

		if (asset->lightmaps)
		{
			AssertSize(Game::IW3::GfxLightmapArray, 8);
			buffer->saveArray(asset->lightmaps, asset->lightmapCount);

			for (int i = 0; i < asset->lightmapCount; ++i)
			{
				Game::IW3::GfxLightmapArray* lightmapArray = &asset->lightmaps[i];

				if (lightmapArray->primary)
				{
					buffer->saveString(lightmapArray->primary->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { lightmapArray->primary });
				}

				if (lightmapArray->secondary)
				{
					buffer->saveString(lightmapArray->secondary->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { lightmapArray->secondary });
				}
			}
		}

		if (asset->skyImage)
		{
			buffer->saveString(asset->skyImage->name);
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { asset->skyImage });
		}

		if (asset->outdoorImage)
		{
			buffer->saveString(asset->outdoorImage->name);
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { asset->outdoorImage });
		}

		// saveGfxWorldVertexData
		{
			if (asset->vd.vertices)
			{
				AssertSize(Game::IW3::GfxWorldVertex, 44);
				buffer->saveArray(asset->vd.vertices, asset->vertexCount);
			}
		}

		// saveGfxWorldVertexLayerData
		{
			if (asset->vld.data)
			{
				// no align for char
				buffer->saveArray(asset->vld.data, asset->vertexLayerDataSize);
			}
		}

		if (asset->indices)
		{
			buffer->saveArray(asset->indices, asset->indexCount);
		}
	}

	void IGfxWorld::SaveGfxWorldDpvsPlanes(Game::IW4::GfxWorld* world, Game::IW3::GfxWorldDpvsPlanes* asset, Utils::Stream* buffer)
	{
		AssertSize(Game::IW3::GfxWorldDpvsPlanes, 16);

		if (asset->planes)
		{
			for (int i = 0; i < world->planeCount; ++i)
			{
				buffer->saveObject(asset->planes[i]);
			}
		}

		if (asset->nodes)
		{
			buffer->saveArray(asset->nodes, world->nodeCount);
		}
	}

	void IGfxWorld::SaveConvertedWorld(Game::IW4::GfxWorld* asset)
	{
		Utils::Stream buffer;
		buffer.saveArray("IW4xGfxW", 8);
		buffer.saveObject(IW4X_GFXMAP_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->baseName)
		{
			buffer.saveString(asset->baseName);
		}

		if (asset->skies)
		{
			AssertSize(Game::IW4::GfxSky, 16);

			buffer.saveArray(asset->skies, asset->skyCount);

			for (int i = 0; i < asset->skyCount; ++i)
			{
				Game::IW4::GfxSky* sky = &asset->skies[i];

				if (sky->skyStartSurfs)
				{
					buffer.saveArray(sky->skyStartSurfs, sky->skySurfCount);
				}

				if (sky->skyImage)
				{
					buffer.saveString(sky->skyImage->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { sky->skyImage });
				}
			}
		}

		IGfxWorld::SaveGfxWorldDpvsPlanes(asset, &asset->dpvsPlanes, &buffer);

		int cellCount = asset->dpvsPlanes.cellCount;

		if (asset->aabbTreeCounts)
		{
			AssertSize(Game::IW4::GfxCellTreeCount, 4);
			buffer.saveArray(asset->aabbTreeCounts, cellCount);
		}

		if (asset->aabbTrees)
		{
			AssertSize(Game::IW4::GfxCellTree, 4);
			buffer.saveArray(asset->aabbTrees, cellCount);

			for (int i = 0; i < cellCount; ++i)
			{
				Game::IW4::GfxCellTree* cellTree = &asset->aabbTrees[i];

				if (cellTree->aabbTree)
				{
					AssertSize(Game::IW3::GfxAabbTree, 44);
					AssertSize(Game::IW4::GfxAabbTree, 44);
					buffer.saveArray(cellTree->aabbTree, asset->aabbTreeCounts[i].aabbTreeCount);

					for (int j = 0; j < asset->aabbTreeCounts[i].aabbTreeCount; ++j)
					{
						Game::IW4::GfxAabbTree* aabbTree = &cellTree->aabbTree[j];

						if (aabbTree->smodelIndexes)
						{
							for (unsigned short k = 0; k < aabbTree->smodelIndexCount; ++k)
							{
								buffer.saveObject(aabbTree->smodelIndexes[k]);
							}
						}
					}
				}
			}
		}

		if (asset->cells)
		{
			AssertSize(Game::IW4::GfxCell, 40);
			buffer.saveArray(asset->cells, cellCount);

			for (int i = 0; i < cellCount; ++i)
			{
				Game::IW4::GfxCell* cell = &asset->cells[i];

				if (cell->portals)
				{
					AssertSize(Game::IW4::GfxPortal, 60);
					buffer.saveArray(cell->portals, cell->portalCount);

					for (int j = 0; j < cell->portalCount; ++j)
					{
						Game::IW4::GfxPortal* portal = &cell->portals[j];

						if (portal->vertices)
						{
							buffer.saveArray(portal->vertices, portal->vertexCount);
						}
					}
				}

				if (cell->reflectionProbes)
				{
					// no align for char
					buffer.saveArray(cell->reflectionProbes, cell->reflectionProbeCount);
				}
			}
		}

		IGfxWorld::SaveGfxWorldDraw(&asset->draw, &buffer);
		IGfxWorld::SaveGfxLightGrid(&asset->lightGrid, &buffer);

		if (asset->models)
		{
			AssertSize(Game::IW4::GfxBrushModel, 60);
			buffer.saveArray(asset->models, asset->modelCount);
		}

		if (asset->materialMemory)
		{
			AssertSize(Game::IW3::MaterialMemory, 8);
			buffer.saveArray(asset->materialMemory, asset->materialMemoryCount);

			for (int i = 0; i < asset->materialMemoryCount; ++i)
			{
				Game::IW3::MaterialMemory* materialMemory = &asset->materialMemory[i];

				if (materialMemory->material)
				{
					buffer.saveString(materialMemory->material->info.name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { materialMemory->material });
				}
			}
		}

		if (asset->sun.spriteMaterial)
		{
			buffer.saveString(asset->sun.spriteMaterial->info.name);
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { asset->sun.spriteMaterial });
		}

		if (asset->sun.flareMaterial)
		{
			buffer.saveString(asset->sun.flareMaterial->info.name);
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_MATERIAL, { asset->sun.flareMaterial });
		}

		if (asset->outdoorImage)
		{
			buffer.saveString(asset->outdoorImage->name);
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { asset->outdoorImage });
		}

		if (asset->shadowGeom)
		{
			AssertSize(Game::IW3::GfxShadowGeometry, 12);
			buffer.saveArray(asset->shadowGeom, asset->primaryLightCount);

			for (unsigned int i = 0; i < asset->primaryLightCount; ++i)
			{
				Game::IW3::GfxShadowGeometry* shadowGeometry = &asset->shadowGeom[i];

				if (shadowGeometry->sortedSurfIndex)
				{
					buffer.saveArray(shadowGeometry->sortedSurfIndex, shadowGeometry->surfaceCount);
				}

				if (shadowGeometry->smodelIndex)
				{
					buffer.saveArray(shadowGeometry->smodelIndex, shadowGeometry->smodelCount);
				}
			}
		}

		if (asset->lightRegion)
		{
			AssertSize(Game::IW3::GfxLightRegion, 8);
			buffer.saveArray(asset->lightRegion, asset->primaryLightCount);

			for (unsigned int i = 0; i < asset->primaryLightCount; ++i)
			{
				Game::IW3::GfxLightRegion* lightRegion = &asset->lightRegion[i];

				if (lightRegion->hulls)
				{
					AssertSize(Game::IW3::GfxLightRegionHull, 80);
					buffer.saveArray(lightRegion->hulls, lightRegion->hullCount);

					for (unsigned int j = 0; j < lightRegion->hullCount; ++j)
					{
						Game::IW3::GfxLightRegionHull* lightRegionHull = &lightRegion->hulls[j];

						if (lightRegionHull->axis)
						{
							AssertSize(Game::IW3::GfxLightRegionAxis, 20);
							buffer.saveArray(lightRegionHull->axis, lightRegionHull->axisCount);
						}
					}
				}
			}
		}

		IGfxWorld::SaveGfxWorldDpvsStatic(asset, &asset->dpvs, &buffer);

		// Obsolete, IW3 has no support for that
		if (asset->heroOnlyLights)
		{
			AssertSize(Game::IW4::GfxHeroOnlyLight, 56);
			buffer.saveArray(asset->heroOnlyLights, asset->heroOnlyLightCount);
		}

		Utils::WriteFile(Utils::VA("%s/gfxworld/%s.iw4xGfxWorld", AssetHandler::GetExportPath().data(), asset->baseName), buffer.toBuffer());
	}

	void IGfxWorld::Dump(Game::IW3::GfxWorld* world)
	{
		if (!world) return;
		Utils::Memory::Allocator allocator;

		Game::IW4::GfxSky sky;
		Game::IW4::GfxWorld map;
		ZeroMemory(&sky, sizeof(sky));
		ZeroMemory(&map, sizeof(map));

		map.name = world->name;
		map.baseName = world->baseName;
		map.planeCount = world->planeCount;
		map.nodeCount = world->nodeCount;
		map.surfaceCount = world->surfaceCount;

		map.skyCount = 1;
		map.skies = &sky;

		sky.skyImage = world->skyImage;
		sky.skySamplerState = world->skySamplerState/* & 0xFF*/;
		sky.skyStartSurfs = world->skyStartSurfs;
		sky.skySurfCount = world->skySurfCount;

		map.lastSunPrimaryLightIndex = world->sunPrimaryLightIndex;
		map.primaryLightCount = world->primaryLightCount;

		map.dpvsPlanes = world->dpvsPlanes;

		// AABBTree data is stored as part of the cells.
		// However, in IW4 it's not, so we have to extract the data
		if (world->cells)
		{
			map.aabbTreeCounts = allocator.allocateArray<Game::IW4::GfxCellTreeCount>(world->dpvsPlanes.cellCount);
			map.aabbTrees = allocator.allocateArray<Game::IW4::GfxCellTree>(world->dpvsPlanes.cellCount);
			map.cells = allocator.allocateArray<Game::IW4::GfxCell>(world->dpvsPlanes.cellCount);

			for (int i = 0; i < world->dpvsPlanes.cellCount; ++i)
			{
				map.aabbTreeCounts[i].aabbTreeCount = world->cells[i].aabbTreeCount;

				map.cells[i].bounds.compute(world->cells[i].mins, world->cells[i].maxs); // Verified
				map.cells[i].portalCount = world->cells[i].portalCount;
				map.cells[i].reflectionProbeCount = world->cells[i].reflectionProbeCount;
				map.cells[i].reflectionProbes = world->cells[i].reflectionProbes;

				if (world->cells[i].aabbTree)
				{
					map.aabbTrees[i].aabbTree = allocator.allocateArray<Game::IW4::GfxAabbTree>(world->cells[i].aabbTreeCount);
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
					map.cells[i].portals = allocator.allocateArray<Game::IW4::GfxPortal>(world->cells[i].portalCount);

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
		map.draw.reflectionProbeTextures = world->reflectionProbeTextures;
		map.draw.lightmapCount = world->lightmapCount;
		map.draw.lightmaps = world->lightmaps;
		map.draw.lightmapPrimaryTextures = world->lightmapPrimaryTextures;
		map.draw.lightmapSecondaryTextures = world->lightmapSecondaryTextures;
		map.draw.skyImage = world->skyImage;
		map.draw.outdoorImage = world->outdoorImage;
		map.draw.vertexCount = world->vertexCount;
		map.draw.vd = world->vd;
		map.draw.vertexLayerDataSize = world->vertexLayerDataSize;
		map.draw.vld = world->vld;
		map.draw.indexCount = world->indexCount;
		map.draw.indices = world->indices;

		// Split reflection images and probes
		if (world->reflectionProbes)
		{
			map.draw.reflectionImages = allocator.allocateArray<Game::IW3::GfxImage*>(world->reflectionProbeCount);
			map.draw.reflectionProbes = allocator.allocateArray<Game::IW4::GfxReflectionProbe>(world->reflectionProbeCount);

			for (unsigned int i = 0; i < world->reflectionProbeCount; ++i)
			{
				map.draw.reflectionImages[i] = world->reflectionProbes[i].reflectionImage;

				std::memcpy(map.draw.reflectionProbes[i].origin, world->reflectionProbes[i].origin, sizeof(map.draw.reflectionProbes[i].origin));
			}
		}

		map.lightGrid = world->lightGrid;
		map.modelCount = world->modelCount;

		if (world->models)
		{
			map.models = allocator.allocateArray<Game::IW4::GfxBrushModel>(world->modelCount);

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
		}

		map.bounds.compute(world->mins, world->maxs);

		map.checksum = world->checksum;
		map.materialMemoryCount = world->materialMemoryCount;
		map.materialMemory = world->materialMemory;
		map.sun = world->sun;

		std::memcpy(map.outdoorLookupMatrix, world->outdoorLookupMatrix, sizeof(map.outdoorLookupMatrix));
		map.outdoorImage = world->outdoorImage;

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
			map.dpvs.smodelInsts = allocator.allocateArray<Game::IW4::GfxStaticModelInst>(world->dpvs.smodelCount);

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
			map.dpvs.surfaces = allocator.allocateArray<Game::IW4::GfxSurface>(world->surfaceCount);
			map.dpvs.surfacesBounds = allocator.allocateArray<Game::IW4::GfxSurfaceBounds>(world->surfaceCount);

			for (int i = 0; i < world->surfaceCount; ++i)
			{
				map.dpvs.surfaces[i].tris = world->dpvs.surfaces[i].tris;
				map.dpvs.surfaces[i].material = world->dpvs.surfaces[i].material;
				map.dpvs.surfaces[i].lightmapIndex = world->dpvs.surfaces[i].lightmapIndex;
				map.dpvs.surfaces[i].reflectionProbeIndex = world->dpvs.surfaces[i].reflectionProbeIndex;
				map.dpvs.surfaces[i].primaryLightIndex = world->dpvs.surfaces[i].primaryLightIndex;
				map.dpvs.surfaces[i].flags = world->dpvs.surfaces[i].flags;

				map.dpvs.surfacesBounds[i].bounds.compute(world->dpvs.surfaces[i].bounds[0], world->dpvs.surfaces[i].bounds[1]); // Verified
			}
		}

		if (world->dpvs.smodelDrawInsts)
		{
			map.dpvs.smodelDrawInsts = allocator.allocateArray<Game::IW4::GfxStaticModelDrawInst>(world->dpvs.smodelCount);

			for (unsigned int i = 0; i < world->dpvs.smodelCount; ++i)
			{
				std::memcpy(map.dpvs.smodelDrawInsts[i].placement.origin, world->dpvs.smodelDrawInsts[i].placement.origin, sizeof(map.dpvs.smodelDrawInsts[i].placement.origin));
				std::memcpy(map.dpvs.smodelDrawInsts[i].placement.axis, world->dpvs.smodelDrawInsts[i].placement.axis, sizeof(map.dpvs.smodelDrawInsts[i].placement.axis));

				std::memcpy(map.dpvs.smodelDrawInsts[i].cacheId, world->dpvs.smodelDrawInsts[i].smodelCacheIndex, sizeof(map.dpvs.smodelDrawInsts[i].cacheId));

				map.dpvs.smodelDrawInsts[i].placement.scale = world->dpvs.smodelDrawInsts[i].placement.scale;
				map.dpvs.smodelDrawInsts[i].model = world->dpvs.smodelDrawInsts[i].model;


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
				map.dpvs.smodelDrawInsts[i].flags = world->dpvs.smodelDrawInsts[i].flags;

				// This has been moved
				if (world->dpvs.smodelInsts)
				{
					map.dpvs.smodelDrawInsts[i].groundLighting = world->dpvs.smodelInsts[i].groundLighting;
					Game::IW3::GfxColor* color = &map.dpvs.smodelDrawInsts[i].groundLighting;

					//// Grass needs 0x20 otherwise it doesn't read data from the lightmap and it's full bright !
					//// Whenever a model needs ground lighting in iw4, it has to specify it
					if (map.dpvs.smodelDrawInsts[i].groundLighting.packed > 0)
					{
						// Color correction to take in account the stronger sun in iw4
						/*
						IW3	IW4
						0	0
						5	4
						15	13
						50	42
						80	66
						120	95
						160	122
						200	147
						255	179
						*/
						unsigned char alpha = color->array[3];
						color->array[3] = static_cast<unsigned char>(
							std::ceil(
								std::log2(alpha / (255 + 150) + 1)
								* 255
							)
							);

						map.dpvs.smodelDrawInsts[i].flags |= 0x20;
						Logger::Print("Added STATIC_MODEL_FLAG_GROUND_LIGHTING on draw instance of %s\n", map.dpvs.smodelDrawInsts[i].model->name);
					}

					// Note: The following is not useful, but it might be one day
					// It is not useful in preventing some objects from turning black. They turn blakc because
					//	they are placed under the ground, which makes the game forbide them from accessing
					//	ground_lighting which they need. Not sure how to detect & fix that !
#if 0
					// Remap everybody above 15 because very dark smodels are actually completely black in iw4
					// Except the alpha! don't touch the alpha!
					for (size_t j = 0; j < 3; j++)
					{
						constexpr auto remapMin = 15;
						auto b = color->array[j];
						color->array[j] = static_cast<unsigned char>(
							std::floor(
								remapMin + (color->array[j] / (255.f + remapMin)) * 255.f
							)
							);

						auto a = color->array[j];
						assert(b <= a); // this cannot darken anyone!
					}
#endif

					}
				}
		}


		map.dpvs.surfaceMaterials = allocator.allocateArray<Game::IW4::GfxDrawSurf>(world->surfaceCount);
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
		map.draw.indices = allocator.allocateArray<unsigned short>(map.draw.indexCount);
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
		map.checksum = 0xC0D40000;

		IGfxWorld::SaveConvertedWorld(&map);
		}

	IGfxWorld::IGfxWorld()
	{
		Command::Add("dumpGfxWorld", [](Command::Params params)
			{
				if (params.Length() < 2) return;
				IGfxWorld::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_GFXWORLD, params[1]).gfxWorld);
			});
	}

	IGfxWorld::~IGfxWorld()
	{

	}
	}
