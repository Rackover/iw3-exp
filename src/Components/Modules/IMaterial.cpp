#include "STDInclude.hpp"

#define IW4X_MAT_VERSION 1

namespace Components
{
	std::unordered_map<char, Game::IW4::Material> IMaterial::exampleMaterialForKey{};
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
		{32, 17},	// Multiplicative, verified on mp_countdown
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
		{43, 29},	// Blend / additive => to a decal layer (shouldn't this be to 47?)
		{48, 48},	// Effect auto sort!
		{56, 49},	// AE Bottom
		{57, 50},	// AE Middle
		{58, 51},	// AE top
		{59, 53}	// Viewmodel effect
	};

	Game::IW4::Material* IMaterial::Convert(Game::IW3::Material* material)
	{
		if (!material)  return nullptr;

        Game::IW4::GfxDrawSurf newSurf;

		Game::IW4::Material mat;
		ZeroMemory(&mat, sizeof mat);

		mat.name = material->info.name;

		mat.gameFlags.packed = material->info.gameFlags.packed;
		mat.gameFlags.fields.unk7 = material->info.gameFlags.fields.unkNeededForSModelDisplay;
		mat.gameFlags.fields.unkCastShadowMaybe = material->info.gameFlags.fields.MTL_GAMEFLAG_CASTS_SHADOW;

		// Sort key
#if USE_IW3_SORTKEYS
		mat.sortKey = material->info.sortKey; // Using iw3 value directly
#else
		mat.sortKey = GetConvertedSortKey(material);
#endif

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

		// Set them all to -1 so they're not used if they dont exist in iw3
		std::memset(mat.stateBitsEntry, 0xFF, sizeof(mat.stateBitsEntry));

		// copy techniques to correct spots
		for (size_t i = 0; i < Game::IW4::TECHNIQUE_COUNT; i++)
		{
			Game::IW4::MaterialTechniqueType technique = static_cast<Game::IW4::MaterialTechniqueType>(i);
			if (IMaterialTechniqueSet::techniqueTypeTableFromIW4.contains(technique))
			{
				mat.stateBitsEntry[technique] = material->stateBitsEntry[IMaterialTechniqueSet::techniqueTypeTableFromIW4.at(technique)];
			}
			else
			{
				// Not necessary
				//mat.stateBitsEntry[technique] = 0xFF;
			}
		}

		mat.textureCount   = material->textureCount;
		mat.constantCount  = material->constantCount;
		mat.stateBitsCount = material->stateBitsCount;
		mat.stateFlags = static_cast<Game::IW4::StateFlags>(material->stateFlags); // Correspondance is identical

		if (mat.sortKey == 0)
		{
			mat.stateFlags = static_cast<Game::IW4::StateFlags>(mat.stateFlags | Game::IW4::STATE_FLAG_AMBIENT);
			Logger::Print("Added STATE_FLAG_AMBIENT on %s (sortkey is opaque-ambient)\n", mat.name);
		}

		// Should we give ambient stateflag to everybody by default?
		mat.cameraRegion   = material->cameraRegion;

		if (mat.cameraRegion == 0x3) {
			// 0x3 is NONE in iw3, but DEPTH_HACK in iw4
			// In iw4 NONE is 0x4
			Logger::Print("Swapped material % s camera region from 0x3 to 0x4 (NONE)\n", mat.name);
			mat.cameraRegion = 0x4;
		}

		mat.techniqueSet = AssetHandler::Convert(Game::IW3::XAssetType::ASSET_TYPE_TECHNIQUE_SET, { material->techniqueSet }).techniqueSet;


		mat.textureTable = LocalAllocator.AllocateArray<Game::IW4::MaterialTextureDef>(mat.textureCount);
		for (char i = 0; i < mat.textureCount; i++)
		{
			auto iw3Def = &material->textureTable[i];
			auto iw4Def = &mat.textureTable[i];

			static_assert(sizeof Game::IW4::MaterialTextureDef == sizeof Game::IW3::MaterialTextureDef);
			std::memcpy(iw4Def, iw3Def, sizeof Game::IW4::MaterialTextureDef);


			if (iw4Def->semantic == 0xB) // TS_Water
			{
				iw4Def->u.water = LocalAllocator.Allocate<Game::IW4::water_t>();

				static_assert(sizeof Game::IW4::water_t == sizeof Game::IW3::water_t);
				std::memcpy(iw4Def->u.water, iw3Def->u.water, sizeof Game::IW4::water_t);

				iw4Def->u.water->image = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { iw3Def->u.water->image }).image;
			}
			else
			{
				iw4Def->u.image = AssetHandler::Convert(Game::IW3::ASSET_TYPE_IMAGE, { iw3Def->u.image }).image;
			}
		}

		mat.constantTable = nullptr;
		if (material->constantTable)
		{
			mat.constantTable = LocalAllocator.AllocateArray<Game::IW3::MaterialConstantDef>(material->constantCount);

			for (char i = 0; i < material->constantCount; ++i)
			{
				Game::IW3::MaterialConstantDef* constantDef = &material->constantTable[i];
				Game::IW3::MaterialConstantDef* targetDef = &mat.constantTable[i];

				std::memcpy(targetDef, constantDef, sizeof(Game::IW3::MaterialConstantDef));

				if (targetDef->name == "envMapParms"s)
				{
					// These use the speculars to add some rimlight effects to models
					// But since speculars are regenerated we end up with cod6 speculars with cod4 materials
					// and cod6 speculars are a bit too bright for 

					targetDef->literal[0] *= 0.0875f; // envMapMin
					targetDef->literal[1] *= 0.2f;  // envMapMax
					targetDef->literal[2] *= 1.4f;    // engMapExponent
					targetDef->literal[3] *= 2.2f;    // envMapIntensity
				}

			}
		}

		mat.stateBitTable  = material->stateBitsTable;

		// Glass
		// this part is like, totally unnecessary. I thought it fixed an issue on broadcast
		// but then it clearly didn't. So not sure we should keep it.
		// Also check via surfacetypebits is big bad
		if (mat.surfaceTypeBits & 0b00000000000000000000000100000000)
		{
			auto index = mat.stateBitsEntry[Game::IW4::TECHNIQUE_LIT];
			auto entry = mat.stateBitTable[index];

			entry.loadbits[0] &= ~(Game::IW3::GFXS0_CULL_MASK | Game::IW3::GFXS0_CULL_BACK | Game::IW3::GFXS0_CULL_FRONT);
			entry.loadbits[0] |= Game::IW3::GFXS0_CULL_NONE;

			Logger::Print("Set statebit %i loadbit 0 to GFXS0_CULL_NONE on material %s (it is glass)\n", index, mat.name);
		}


#if not USE_IW3_SORTKEYS
		Game::IW4::Material conflict;
		if (CheckSortKeyConflict(&mat, conflict))
		{
			Components::Logger::Print("There is a sort key conflict between %s and %s on the key %i. One of these two (most probably the first one) doesn't have the right sort key! You will need to fix the material JSON manually.\n",
				mat.name, conflict.name, mat.sortKey);
			assert(false);
		}
#endif

		Game::IW4::Material* iw4Material = LocalAllocator.Allocate<Game::IW4::Material>();
		*iw4Material = mat;

		return iw4Material;
	}

	char IMaterial::GetConvertedSortKey(Game::IW3::Material* material) 
	{
		char iw3Key = material->info.sortKey;

		const std::string& name = material->info.name;
		const std::string& techsetName = material->techniqueSet->name;

		if (iw3Key == 4) 
		{
			// This takes care of two building facades
			if (techsetName.contains("ambient"s))
			{
				// Opaque => Opaque_Ambient
				Logger::Print("Material %s was given sortkey %i from %i (techset name contains 'ambient')\n", name.data(), 0, iw3Key);
				return 0;
			}

			// This takes care of multiple gfx, like dust_mote, aswell as other special materials
			if (!name.starts_with("mc") && !name.starts_with("wc"))
			{
				Logger::Print("Material %s was given sortkey %i from %i (not mc nor wc)\n", name.data(), 34, iw3Key);
				return 34;
			}

			if (name.contains("shadow"))
			{
				Logger::Print("Material %s was given sortkey %i from %i (contains 'shadow' in the name, likely shadowcaster)\n", name.data(), 34, iw3Key);
				return 34;
			}

			if (name.contains("mtl_fx") && !(material->stateFlags & Game::IW3::StateFlags::STATE_FLAG_WRITES_DEPTH))
			{
				// Not good
				Logger::Print("Material %s was given sortkey %i from %i (does not write to depth)\n", name.data(), 29, iw3Key);
				return 29; // mtl_fx_rock01
			}

			// Why ? How!
			// mtl_bm21_undercarriage what's wrong with you!
			if (techsetName.contains("2d"))
			{
				//"opaque" => SORTKEY_BLEND_ADDITIVE
				Logger::Print("Material %s was given sortkey %i from %i (2d)\n", name.data(), 47, iw3Key);
				return 47;
			}
		}

		if (iw3Key == 9)
		{
			// Ahh I hate this
			// But there is no difference between sign 02 01 and 03, and they're all
			// on sortkey 6 except 02! which is on sortkey 10
			// Who can tell why?
			if (name == "me_signs_02"s) 
			{
				Logger::Print("Material %s was given sortkey %i from %i (exceptional)\n", name.data(), 10, iw3Key);
				return 10;
			}
		}

		// decal - static decal got broken down in multiple categorries
		if (iw3Key == 12)
		{
			if (techsetName.contains("shadow"s))
			{
				/*
				 - 34 (4 matches)
					 - mp_crash:mc\mtl_tree_shadow_caster.json
					 - mp_overgrown:mc\mtl_tree_shadow_caster.json
					 - mp_strike:mc\mtl_tree_shadow_caster.json
					 - mp_vacant:mc\mtl_tree_shadow_caster.json
				 */

				Logger::Print("Material %s was given sortkey %i from %i (techset name contains 'shadow')\n", name.data(), 34, iw3Key);
				return 34;
			}


			if (techsetName.contains("effect"))
			{
				// Really not sure about this! But some posters on some custom maps (mp_nuked_xmas) are on wc_effect tecshet
				// and cause a sort crash when put on the decal layer (9)
				Logger::Print("Material %s was given sortkey %i from %i (techset name contains 'effect')\n", name.data(), 34, iw3Key);
				return 48;
			}

			/*
			 - 16 (3 matches)
				 - mp_crash:mc\ch_bulletinboardpaperdecals_2.json
				 - mp_strike:mc\ch_bulletinboardpaperdecals_2.json
				 - mp_vacant:mc\ch_bulletinboardpaperdecals_2.json
			 - 6 (3 matches)
				 - mp_crash:wc\ch_dec_darkstain_01.json
				 - mp_overgrown:wc\ch_dec_darkstain_01.json
				 - mp_strike:wc\ch_dec_darkstain_01.json
			 - 7 (2 matches)
				 - mp_crash:wc\ch_rubble_conc01_decal.json
				 - mp_vacant:wc\ch_rubble_conc01_decal.json
			 - 0 (1 matches)
				 - mp_overgrown:mc\mtl_red_pine_canopy.json
			
			*/
		}

		if (iw3Key == 24)
		{
			if (name.contains("glass"))
			{
				/*
			 - 26 (8 matches)
				 - common:mc\gfx_impact_glass01.json
				 - common:mc\gfx_impact_glass02.json
				 - common:wc\gfx_impact_glass01.json
				 - common:wc\gfx_impact_glass02.json
				 - common_mp:mc\gfx_impact_glass01.json
				 - common_mp:mc\gfx_impact_glass02.json
				 - common_mp:wc\gfx_impact_glass01.json
				 - common_mp:wc\gfx_impact_glass02.json
				*/
				Logger::Print("Material %s was given sortkey %i from %i ('glass' in the material name)\n", name.data(), 26, iw3Key);
				return 26;
			}

			if (techsetName.contains("effect"))
			{
				/*
				 - 48 (4 matches)
					 - common:gfx_mud_splash01.json
					 - common:gfx_mud_splash02.json
					 - common_mp:gfx_mud_splash01.json
					 - common_mp:gfx_mud_splash02.json
				*/
				Logger::Print("Material %s was given sortkey %i from %i ('effect' in the techset name)\n", name.data(), 48, iw3Key);
				return 48;
			}
		}

		// Blend / additive
		if (iw3Key == 43)
		{
			// Default 29
			if (techsetName.contains("2d"))
			{
				//"blend / additive" => SORTKEY_BLEND_ADDITIVE
				Logger::Print("Material %s was given sortkey %i from %i (2d)\n", name.data(), 47, iw3Key);
				return 47;
			}

			if (techsetName.contains("effect") && name.contains("gfx"))
			{
				//"blend / additive" => SORTKEY_BLEND_ADDITIVE
				// gfx_flare_wtr is an example of this
				Logger::Print("Material %s was given sortkey %i from %i (is a gfx/effect material)\n", name.data(), 47, iw3Key);
				return 47;
			}

			if (techsetName.contains("particle"))
			{
				// cloud_snowflake01
				if (techsetName.contains("outdoor"))
				{
					Logger::Print("Material %s was given sortkey %i from %i (is an outdoor particle)\n", name.data(), 51, iw3Key);
					return 51;
				}
				else
				{
					Logger::Print("Material %s was given sortkey %i from %i (is a particle)\n", name.data(), 47, iw3Key);
					return 47;
				}
			}


#define IW3_CAMERA_REGION_DECAL 0x1
			if (material->cameraRegion == IW3_CAMERA_REGION_DECAL)
			{
				/*
			 - 25 (2 matches)
				 - common_mp:mc\mtl_weapon_reflex_lens.json
				 - mp_overgrown:mc\mtl_vehicle_tractor_glass.json
				*/
				Logger::Print("Material %s was given sortkey %i from %i (0x1 camera region (decal))\n", name.data(), 25, iw3Key);
				return 25;
			}

			if (techsetName.contains("tools"))
			{
				//"opaque" => SORTKEY_BLEND_ADDITIVE
				Logger::Print("Material %s was given sortkey %i from %i (tools)\n", name.data(), 47, iw3Key);
				return 47;
			}

			/*
			 - 34 (4 matches)
				 - mp_crash_load:$victorybackdrop.json
				 - mp_overgrown_load:$victorybackdrop.json
				 - mp_strike_load:$victorybackdrop.json
				 - mp_vacant_load:$victorybackdrop.json
			 - 48 (3 matches)
				 - common_mp:gfx_sun_flare_toujane.json
				 - mp_overgrown:sun_flare_rainbow.json
				 - mp_vacant:sun_flare_rainbow.json
			 - 9 (1 matches)
				 - mp_vacant:mc\mtl_cardboardbox_decal.json
			*/
		}



		if (IMaterial::sortKeysTable.contains(iw3Key)) {
			return IMaterial::sortKeysTable[iw3Key];
		}

		Components::Logger::Print(Utils::VA("Could not find a fitting key for sortkey %i in material %s\n", iw3Key, material->info.name));

		return iw3Key;
	}

	bool IMaterial::CheckSortKeyConflict(Game::IW4::Material* material, OUT Game::IW4::Material& conflictingMaterial)
	{
		if (exampleMaterialForKey.find(material->sortKey) == exampleMaterialForKey.end())
		{
			exampleMaterialForKey[material->sortKey] = *material;
			return false;
		}

		auto* otherMaterial = &exampleMaterialForKey.at(material->sortKey);

		assert(material->techniqueSet);

		bool hasLit = material->techniqueSet->techniques[Game::IW4::TECHNIQUE_LIT] != nullptr;
		bool otherHasLit = otherMaterial->techniqueSet->techniques[Game::IW4::TECHNIQUE_LIT] != nullptr;

		if (otherHasLit != hasLit)
		{
			conflictingMaterial = *otherMaterial;
			return true;
		}

		if (hasLit)
		{
			bool hasEmissive = material->techniqueSet->techniques[Game::IW4::TECHNIQUE_EMISSIVE];
			bool otherHasEmissive = otherMaterial->techniqueSet->techniques[Game::IW4::TECHNIQUE_EMISSIVE];

			if (hasEmissive)
			{
				conflictingMaterial = *otherMaterial;
				return true;
			}

			if (otherHasEmissive)
			{
				conflictingMaterial = *otherMaterial;
				return true;
			}
		}
			

		return false;
	}

	IMaterial::IMaterial()
	{
		Command::Add("dumpMaterial", [](const Command::Params& params)
		{
			if (params.Length() < 2) return;
			auto asset = IMaterial::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_MATERIAL, params[1]).material);
			MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_MATERIAL, asset);
		});
	}

	IMaterial::~IMaterial()
	{
	}
}
