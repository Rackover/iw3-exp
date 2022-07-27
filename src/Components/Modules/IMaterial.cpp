#include "STDInclude.hpp"

#define IW4X_MAT_VERSION "1"

namespace Components
{
	std::unordered_map<char, char> IMaterial::sortKeysTable = {
		{0, 43},	// Distortion
					// Opaque water (never used)
					// Boat hull (never used)
		{3, 0},		// Opaque ambient
		{4, 1},		// Opaque
		{5, 2},		// Sky
		{6, 3},		// Skybox sun/moon
		{7, 4},		// Clouds  NOT SURE
		{8, 5},		// Horizon NOT SURE
		{9, 6},		// Decal bottom 1
		{10, 7},	// Decal bottom 2
		{11, 8},	// Decal bottom 3
		{12, 9},	// Decal static
		{13, 10},	// Decal mid 1
		{14, 11},	// Decal mid 2
		{15, 12},	// Decal mid 3
		{24, 13},	// Weapon Impact
		{29, 14},	// Decal top 1
		{30, 15},	// Decal top 2
		{31, 16},	// Decal top 3
		{32, 41},	// Multiplicative
		{33, 17},	// Banner/ Curtain (wild guess!)
		{34, 4 },	// Hair. I matched it with german shepherd material sortkey, i hope its ok.
		{35, 19},	// Underwater
		{36, 20},	// Transparent water
		{37, 33},	// Corona (wild guess)
		{38, 24},	// Window inside
		{39, 25},	// Window outside
		{40, 44},	// Before effects 1 (wild guess)
		{41, 45},	// Before effects 2 (wild guess)
		{42, 46},	// Before effects 3 (extremely wild guess)
		{48, 48},	// Effect auto sort!
		{56, 49},	// AE Bottom
		{57, 50},	// AE Middle
		{58, 51},	// AE top
		{59, 53}	// Viewmodel effect
	};

	void IMaterial::SaveConvertedMaterial(Game::IW4::Material* asset)
	{
		Utils::Stream buffer;
		buffer.saveArray("IW4xMat" IW4X_MAT_VERSION, 8); // just stick version in the magic since we have an extra char

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->techniqueSet)
		{
			buffer.saveString(asset->techniqueSet->name);
            AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET, { asset->techniqueSet });
		}

		if (asset->textureTable)
		{
			buffer.saveArray(asset->textureTable, asset->textureCount);

			for (char i = 0; i < asset->textureCount; ++i)
			{
				Game::IW3::MaterialTextureDef* textureDef = &asset->textureTable[i];

				if (textureDef->semantic == TS_WATER_MAP)
				{
					AssertSize(Game::IW3::water_t, 68);

					Game::IW3::water_t* water = textureDef->u.water;

					if (water)
					{
						buffer.saveObject(*water);

						// Save_water_t
						if (water->H0)
						{
							buffer.saveArray(water->H0, water->M * water->N);
						}

						if (water->wTerm)
						{
							buffer.saveArray(water->wTerm, water->M * water->N);
						}

						if (water->image)
						{
							buffer.saveString(water->image->name);
							AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { water->image });
						}
					}
				}
				else if (textureDef->u.image)
				{
					buffer.saveString(textureDef->u.image->name);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { textureDef->u.image });
				}
			}
		}

		if (asset->constantTable)
		{
			for (char i = 0; i < asset->constantCount; ++i)
			{
				Game::IW3::MaterialConstantDef constantDef;
				std::memcpy(&constantDef, &asset->constantTable[i], sizeof Game::IW3::MaterialConstantDef);

				// This is like the ugliest fix i could come up with.
				// I'm way to sick to even understand what MaterialConstantDef actually contain
				// And for now, this shit seems to work
				if (constantDef.name == "envMapParms"s)
				{
					constantDef.literal[0] *= 0.0875f;
					constantDef.literal[1] *= 0.165f;
					constantDef.literal[2] *= 1.4f;
					constantDef.literal[3] *= 3.2f;
				}

				buffer.saveObject(constantDef);
			}
		}

		if (asset->stateBitTable)
		{
			buffer.saveArray(asset->stateBitTable, asset->stateBitsCount);
		}

		Utils::WriteFile(Utils::VA("%s/materials/%s.iw4xMaterial", AssetHandler::GetExportPath().data(), asset->name), buffer.toBuffer());
	}

	void IMaterial::Dump(Game::IW3::Material* material)
	{
		if (!material) return;
		Utils::Memory::Allocator allocator;

        Game::IW4::GfxDrawSurf newSurf;

		Game::IW4::Material mat;
		ZeroMemory(&mat, sizeof mat);

		mat.name                    = material->info.name;
		mat.gameFlags.packed               = material->info.gameFlags.packed;


		mat.gameFlags.fields.unk8 = material->info.gameFlags.fields.unk7;
		mat.gameFlags.fields.unk7 = material->info.gameFlags.fields.unk8;
			
		mat.sortKey                 = GetConvertedSortKey(material); // Might be recalculated in IW4
		mat.textureAtlasRowCount    = material->info.textureAtlasRowCount;
		mat.textureAtlasColumnCount = material->info.textureAtlasColumnCount;

        newSurf.fields.objectId = material->info.drawSurf.fields.objectId;
        newSurf.fields.reflectionProbeIndex = material->info.drawSurf.fields.reflectionProbeIndex;
        newSurf.fields.customIndex = material->info.drawSurf.fields.customIndex;
        newSurf.fields.materialSortedIndex = material->info.drawSurf.fields.materialSortedIndex;
        newSurf.fields.prepass = material->info.drawSurf.fields.prepass;
        newSurf.fields.sceneLightIndex = material->info.drawSurf.fields.primaryLightIndex;
        newSurf.fields.surfType = material->info.drawSurf.fields.surfType;
        newSurf.fields.primarySortKey = material->info.drawSurf.fields.primarySortKey;
        mat.drawSurf.packed = newSurf.packed;

		mat.surfaceTypeBits         = material->info.surfaceTypeBits;
		mat.hashIndex               = material->info.hashIndex;

		std::memset(mat.stateBitsEntry, 0xFF, sizeof(mat.stateBitsEntry));

		mat.stateBitsEntry[Game::IW4::TECHNIQUE_DEPTH_PREPASS]                  = material->stateBitsEntry[Game::IW3::TECHNIQUE_DEPTH_PREPASS];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_BUILD_FLOAT_Z]                  = material->stateBitsEntry[Game::IW3::TECHNIQUE_BUILD_FLOAT_Z];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_BUILD_SHADOWMAP_DEPTH]          = material->stateBitsEntry[Game::IW3::TECHNIQUE_BUILD_SHADOWMAP_DEPTH];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_BUILD_SHADOWMAP_COLOR]          = material->stateBitsEntry[Game::IW3::TECHNIQUE_BUILD_SHADOWMAP_COLOR];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_UNLIT]                          = material->stateBitsEntry[Game::IW3::TECHNIQUE_UNLIT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_EMISSIVE]                       = material->stateBitsEntry[Game::IW3::TECHNIQUE_EMISSIVE];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_EMISSIVE_DFOG]                  = material->stateBitsEntry[Game::IW3::TECHNIQUE_EMISSIVE];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_EMISSIVE_SHADOW]                = material->stateBitsEntry[Game::IW3::TECHNIQUE_EMISSIVE_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_EMISSIVE_SHADOW_DFOG]           = material->stateBitsEntry[Game::IW3::TECHNIQUE_EMISSIVE_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_BEGIN]                      = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_BEGIN];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT]                            = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_DFOG]                       = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SUN]                        = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SUN];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SUN_DFOG]                   = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SUN];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SUN_SHADOW]                 = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SUN_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SUN_SHADOW_DFOG]            = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SUN_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SPOT]                       = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SPOT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SPOT_DFOG]                  = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SPOT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SPOT_SHADOW]                = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SPOT_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_SPOT_SHADOW_DFOG]           = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_SPOT_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_OMNI]                       = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_OMNI];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_OMNI_DFOG]                  = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_OMNI];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_OMNI_SHADOW]                = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_OMNI_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_OMNI_SHADOW_DFOG]           = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_OMNI_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED]                  = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_DFOG]             = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN]              = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_DFOG]         = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW]       = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG]  = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT]             = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_DFOG]        = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW]      = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG] = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI]             = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_DFOG]        = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW]      = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG] = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT_END]                        = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIT_END];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIGHT_SPOT]                     = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIGHT_SPOT];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIGHT_OMNI]                     = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIGHT_OMNI];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIGHT_SPOT_SHADOW]              = material->stateBitsEntry[Game::IW3::TECHNIQUE_LIGHT_SPOT_SHADOW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_FAKELIGHT_NORMAL]               = material->stateBitsEntry[Game::IW3::TECHNIQUE_FAKELIGHT_NORMAL];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_FAKELIGHT_VIEW]                 = material->stateBitsEntry[Game::IW3::TECHNIQUE_FAKELIGHT_VIEW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_SUNLIGHT_PREVIEW]               = material->stateBitsEntry[Game::IW3::TECHNIQUE_SUNLIGHT_PREVIEW];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_CASE_TEXTURE]                   = material->stateBitsEntry[Game::IW3::TECHNIQUE_CASE_TEXTURE];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_WIREFRAME_SOLID]                = material->stateBitsEntry[Game::IW3::TECHNIQUE_WIREFRAME_SOLID];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_WIREFRAME_SHADED]               = material->stateBitsEntry[Game::IW3::TECHNIQUE_WIREFRAME_SHADED];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_DEBUG_BUMPMAP]                  = material->stateBitsEntry[Game::IW3::TECHNIQUE_DEBUG_BUMPMAP];
		mat.stateBitsEntry[Game::IW4::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED]        = material->stateBitsEntry[Game::IW3::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED];

		mat.textureCount   = material->textureCount;
		mat.constantCount  = material->constantCount;
		mat.stateBitsCount = material->stateBitsCount;
		mat.stateFlags = static_cast<Game::IW4::StateFlags>(material->stateFlags); // Correspondance is identical
		mat.cameraRegion   = material->cameraRegion;



		if (mat.cameraRegion == 0x4) {
			// 0x4 is NONE in iw3, but DEPTH_HACK in iw4
			// In iw4 NONE is 0x5
			Logger::Print(Utils::VA("Swapped material %s camera region from 0x4 to 0x5 (NONE)\n", mat.name));
			mat.cameraRegion = 0x5;
		}

		mat.techniqueSet   = material->techniqueSet;
		mat.textureTable   = material->textureTable;
		mat.constantTable  = material->constantTable;
		mat.stateBitTable  = material->stateBitsTable;

		IMaterial::SaveConvertedMaterial(&mat);
	}

	char IMaterial::GetConvertedSortKey(Game::IW3::Material* material) 
	{
		char iw3Key = material->info.sortKey;

		if (IMaterial::sortKeysTable.contains(iw3Key)) {
			return IMaterial::sortKeysTable[iw3Key];
		}

		Components::Logger::Print(Utils::VA("Could not find a fitting key for sortkey %i in material %s\n", iw3Key, material->info.name));

		return iw3Key;
	}


	IMaterial::IMaterial()
	{
		Command::Add("dumpMaterial", [](Command::Params params)
		{
			if (params.Length() < 2) return;
			IMaterial::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, params[1]).material);
		});
	}

	IMaterial::~IMaterial()
	{
	}
}
