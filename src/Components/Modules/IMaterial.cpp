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

	void IMaterial::SaveConvertedMaterial(Game::IW4::Material* asset)
	{
		Utils::Memory::Allocator strDuplicator;
		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();

		output.AddMember("version", IW4X_MAT_VERSION, allocator);

		output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

		const auto gameFlags = std::format("{:08b}", asset->gameFlags.packed);
		output.AddMember("gameFlags", RAPIDJSON_STR(gameFlags.c_str()), allocator);

		const auto stateFlags = std::format("{:08b}", static_cast<char>(asset->stateFlags));
		output.AddMember("stateFlags", RAPIDJSON_STR(stateFlags.c_str()), allocator);

#define SAME_NAME_JSON_MEMBER(x) output.AddMember(#x, asset->x, allocator)

		SAME_NAME_JSON_MEMBER(sortKey);

		std::string techsetName;
		if (asset->techniqueSet)
		{
			AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET, { asset->techniqueSet });

			techsetName = std::format("{}{}", asset->techniqueSet->name, IMaterialTechniqueSet::techsetSuffix);
			output.AddMember("techniqueSet", RAPIDJSON_STR(techsetName.c_str()), allocator);
		}


		SAME_NAME_JSON_MEMBER(textureAtlasRowCount);
		SAME_NAME_JSON_MEMBER(textureAtlasColumnCount);

		const auto surfaceTypeBits = std::format("{:032b}", asset->surfaceTypeBits);
		output.AddMember("surfaceTypeBits", RAPIDJSON_STR(surfaceTypeBits.c_str()), allocator);

		rapidjson::Value textureTable(rapidjson::kArrayType);

		if (asset->textureTable)
		{
			for (char i = 0; i < asset->textureCount; ++i)
			{
				Game::IW3::MaterialTextureDef* textureDef = &asset->textureTable[i];
				rapidjson::Value textureJson(rapidjson::kObjectType);

				textureJson.AddMember("nameStart", textureDef->nameStart, allocator);
				textureJson.AddMember("nameEnd", textureDef->nameEnd, allocator);
				textureJson.AddMember("nameHash", textureDef->nameHash, allocator);
				textureJson.AddMember("samplerState", textureDef->samplerState, allocator); // $6961E030A9677F7C86FC6FF9B5901495
				textureJson.AddMember("semantic", textureDef->semantic, allocator);

				if (textureDef->semantic == TS_WATER_MAP)
				{
					AssertSize(Game::IW3::water_t, 68);

					Game::IW3::water_t* water = textureDef->u.water;

					if (water)
					{
						rapidjson::Value waterJson(rapidjson::kObjectType);

						if (water->image)
						{
							waterJson.AddMember("image", RAPIDJSON_STR(water->image->name), allocator);
							AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { water->image });
						}

						// Save_water_t
						if (water->H0)
						{
							auto ptr = reinterpret_cast<uint8_t*>(water->H0);
							auto buffer = std::vector<uint8_t>(ptr, ptr + water->M * water->N * sizeof(Game::IW3::complex_s));

							waterJson.AddMember("H0", RAPIDJSON_STR(strDuplicator.duplicateString(Utils::Base64::Encode(buffer))), allocator);
						}

						if (water->wTerm)
						{
							auto ptr = reinterpret_cast<uint8_t*>(water->wTerm);
							auto buffer = std::vector<uint8_t>(ptr, ptr + water->M * water->N * sizeof(float));

							waterJson.AddMember("wTerm", RAPIDJSON_STR(strDuplicator.duplicateString(Utils::Base64::Encode(buffer))), allocator);
						}

						#define SAME_NAME_WATER_MEMBER(x) waterJson.AddMember(#x, water->x, allocator)

						SAME_NAME_WATER_MEMBER(M);
						SAME_NAME_WATER_MEMBER(N);
						SAME_NAME_WATER_MEMBER(Lx);
						SAME_NAME_WATER_MEMBER(Lz);
						SAME_NAME_WATER_MEMBER(gravity);
						SAME_NAME_WATER_MEMBER(windvel);
						waterJson.AddMember("winddir", Utils::MakeJsonArray(water->winddir, 2, allocator), allocator);

						SAME_NAME_WATER_MEMBER(amplitude);
						waterJson.AddMember("codeConstant", Utils::MakeJsonArray(water->codeConstant, 4, allocator), allocator);


						textureJson.AddMember("water", waterJson, allocator);
					}
				}
				else if (textureDef->u.image)
				{
					textureJson.AddMember("image", RAPIDJSON_STR(textureDef->u.image->name), allocator);
					AssetHandler::Dump(Game::XAssetType::ASSET_TYPE_IMAGE, { textureDef->u.image });
				}

				textureTable.PushBack(textureJson, allocator);
			}
		}

		output.AddMember("textureTable", textureTable, allocator);

		rapidjson::Value gfxDrawSurface(rapidjson::kObjectType);

#define SAME_NAME_GFXDRAWSURF_MEMBER(x) gfxDrawSurface.AddMember(#x, asset->drawSurf.fields.##x##, allocator)

		SAME_NAME_GFXDRAWSURF_MEMBER(objectId);
		SAME_NAME_GFXDRAWSURF_MEMBER(reflectionProbeIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(hasGfxEntIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(customIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(materialSortedIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(prepass);
		SAME_NAME_GFXDRAWSURF_MEMBER(useHeroLighting);
		SAME_NAME_GFXDRAWSURF_MEMBER(sceneLightIndex);
		SAME_NAME_GFXDRAWSURF_MEMBER(surfType);
		SAME_NAME_GFXDRAWSURF_MEMBER(primarySortKey);
		SAME_NAME_GFXDRAWSURF_MEMBER(unused);

		output.AddMember("gfxDrawSurface", gfxDrawSurface, allocator);

		SAME_NAME_JSON_MEMBER(hashIndex);

		rapidjson::Value stateBitsEntry(rapidjson::kArrayType);
		for (size_t i = 0; i < 48; i++)
		{
			stateBitsEntry.PushBack(asset->stateBitsEntry[i], allocator);
		}

		output.AddMember("stateBitsEntry", stateBitsEntry, allocator);

		SAME_NAME_JSON_MEMBER(cameraRegion);

		if (asset->constantTable)
		{
			rapidjson::Value constantTable(rapidjson::kArrayType);

			for (char i = 0; i < asset->constantCount; ++i)
			{
				Game::IW3::MaterialConstantDef constantDef;
				std::memcpy(&constantDef, &asset->constantTable[i], sizeof Game::IW3::MaterialConstantDef);

				rapidjson::Value constantDefJson(rapidjson::kObjectType);

				if (constantDef.name == "envMapParms"s)
				{
					// These use the speculars to add some rimlight effects to models
					// But since speculars are regenerated we end up with cod6 speculars with cod4 materials
					// and cod6 speculars are a bit too bright for 

					constantDef.literal[0] *= 1.2f; // envMapMin
					constantDef.literal[1] *= 0.2f;  // envMapMax
					constantDef.literal[2] *= 1.4f;    // engMapExponent
					constantDef.literal[3] *= 1.2f;    // envMapIntensity
				}

				// "detailScale" might need some work too 🤔

				constantDefJson.AddMember("nameHash", constantDef.nameHash, allocator);
				constantDefJson.AddMember("literal", Utils::MakeJsonArray(constantDef.literal, 4, allocator), allocator);


				std::string constantDefName = constantDef.name;
				constantDefName = constantDefName.substr(0, 12);

				constantDefJson.AddMember("name", RAPIDJSON_STR(strDuplicator.duplicateString(constantDefName.c_str())), allocator);


				constantTable.PushBack(constantDefJson, allocator);
			}

			output.AddMember("constantTable", constantTable, allocator);
		}

		if (asset->stateBitTable)
		{
			output.AddMember("stateBitsTable", StateBitsToJsonArray(asset->stateBitTable, asset->stateBitsCount, allocator), allocator);
		}

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		Utils::WriteFile(Utils::VA("%s/materials/%s.iw4x.json", AssetHandler::GetExportPath().data(), asset->name), buff.GetString());
	}

	rapidjson::Value IMaterial::StateBitsToJsonArray(Game::IW3::GfxStateBits* stateBits, unsigned char count, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) 
	{
		rapidjson::Value arr(rapidjson::kArrayType);

		for (auto index = 0u; index < count; index++)
		{
			const auto& entry = stateBits[index];

			const auto srcBlendRgb = (entry.flags.loadbit0 & Game::IW3::GFXS0_SRCBLEND_RGB_MASK) >> Game::IW3::GFXS0_SRCBLEND_RGB_SHIFT;
			const auto dstBlendRgb = (entry.flags.loadbit0 & Game::IW3::GFXS0_DSTBLEND_RGB_MASK) >> Game::IW3::GFXS0_DSTBLEND_RGB_SHIFT;
			const auto blendOpRgb = (entry.flags.loadbit0 & Game::IW3::GFXS0_BLENDOP_RGB_MASK) >> Game::IW3::GFXS0_BLENDOP_RGB_SHIFT;
			const auto srcBlendAlpha = (entry.flags.loadbit0 & Game::IW3::GFXS0_SRCBLEND_ALPHA_MASK) >> Game::IW3::GFXS0_SRCBLEND_ALPHA_SHIFT;
			const auto dstBlendAlpha = (entry.flags.loadbit0 & Game::IW3::GFXS0_DSTBLEND_ALPHA_MASK) >> Game::IW3::GFXS0_DSTBLEND_ALPHA_SHIFT;
			const auto blendOpAlpha = (entry.flags.loadbit0 & Game::IW3::GFXS0_BLENDOP_ALPHA_MASK) >> Game::IW3::GFXS0_BLENDOP_ALPHA_SHIFT;
			const auto depthTest = (entry.flags.loadbit1 & Game::IW3::GFXS1_DEPTHTEST_DISABLE) ? -1 : (entry.flags.loadbit1 & Game::IW3::GFXS1_DEPTHTEST_MASK) >> Game::IW3::GFXS1_DEPTHTEST_SHIFT;
			const auto polygonOffset = (entry.flags.loadbit1 & Game::IW3::GFXS1_POLYGON_OFFSET_MASK) >> Game::IW3::GFXS1_POLYGON_OFFSET_SHIFT;

			const auto* alphaTest = "disable"; 
			if ((entry.flags.loadbit0 & Game::IW3::GFXS0_ATEST_MASK) == Game::IW3::GFXS0_ATEST_GE_128)
			{
				alphaTest = ">=128";
			}
			else if ((entry.flags.loadbit0 & Game::IW3::GFXS0_ATEST_MASK) == Game::IW3::GFXS0_ATEST_GT_0)
			{
				alphaTest = ">0";
			}
			else if ((entry.flags.loadbit0 & Game::IW3::GFXS0_ATEST_MASK) == Game::IW3::GFXS0_ATEST_LT_128)
			{
				alphaTest = "<128";
			}
			else
			{
				assert(entry.flags.loadbit0 & Game::IW3::GFXS0_ATEST_DISABLE);
			}

			const auto* cullFace = "none";
			if ((entry.flags.loadbit0 & Game::IW3::GFXS0_CULL_MASK) == Game::IW3::GFXS0_CULL_BACK)
			{
				cullFace = "back";
			}
			else if ((entry.flags.loadbit0 & Game::IW3::GFXS0_CULL_MASK) == Game::IW3::GFXS0_CULL_FRONT)
			{
				cullFace = "front";
			}
			else
			{
				assert((entry.flags.loadbit0 & Game::IW3::GFXS0_CULL_MASK) == Game::IW3::GFXS0_CULL_NONE);
			}

			rapidjson::Value stateBitEntry(rapidjson::kObjectType);
			
			const auto colorWriteRgb = entry.flags.loadbit0 & Game::IW3::GFXS0_COLORWRITE_RGB ? true : false;
			const auto colorWriteAlpha = entry.flags.loadbit0 & Game::IW3::GFXS0_COLORWRITE_ALPHA ? true : false;
			const auto polymodeLine = entry.flags.loadbit0 & Game::IW3::GFXS0_POLYMODE_LINE ? true : false;

			// Missing gamma write here, present in iw4 and absent in iw3
			const auto gammaWrite = false; // Makes everything too bright if we set it to true.

			const auto depthWrite = (entry.flags.loadbit1 & Game::IW3::GFXS1_DEPTHWRITE) ? true : false;
			const auto stencilFrontEnabled = (entry.flags.loadbit1 & Game::IW3::GFXS1_STENCIL_FRONT_ENABLE) ? true : false;
			const auto stencilBackEnabled = (entry.flags.loadbit1 & Game::IW3::GFXS1_STENCIL_BACK_ENABLE) ? true : false;
			const auto stencilFrontPass = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_FRONT_PASS_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;
			const auto stencilFrontFail = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_FRONT_FAIL_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;
			const auto stencilFrontZFail = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_FRONT_ZFAIL_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;
			const auto stencilFrontFunc = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_FRONT_FUNC_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;
			const auto stencilBackPass = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_BACK_PASS_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;
			const auto stencilBackFail = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_BACK_FAIL_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;
			const auto stencilBackZFail = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_BACK_ZFAIL_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;
			const auto stencilBackFunc = (entry.flags.loadbit1 >> Game::IW3::GFXS1_STENCIL_BACK_FUNC_SHIFT) & Game::IW3::GFXS_STENCILOP_MASK;

#define ADD_TO_JSON(x) stateBitEntry.AddMember(#x, x, allocator)
#define ADD_TO_JSON_STR(x) stateBitEntry.AddMember(#x, RAPIDJSON_STR(x), allocator)

			ADD_TO_JSON_STR(alphaTest);
			ADD_TO_JSON(blendOpAlpha);
			ADD_TO_JSON(blendOpRgb);
			ADD_TO_JSON(colorWriteAlpha);
			ADD_TO_JSON(colorWriteRgb);
			ADD_TO_JSON_STR(cullFace);
			ADD_TO_JSON(depthTest);
			ADD_TO_JSON(depthWrite);
			ADD_TO_JSON(dstBlendAlpha);
			ADD_TO_JSON(dstBlendRgb);
			ADD_TO_JSON(gammaWrite);
			ADD_TO_JSON(polygonOffset);
			ADD_TO_JSON(polymodeLine);
			ADD_TO_JSON(srcBlendRgb);
			ADD_TO_JSON(srcBlendAlpha);
			ADD_TO_JSON(stencilBackEnabled);
			ADD_TO_JSON(stencilBackFail);
			ADD_TO_JSON(stencilBackFunc);
			ADD_TO_JSON(stencilBackPass);
			ADD_TO_JSON(stencilBackZFail);
			ADD_TO_JSON(stencilFrontEnabled);
			ADD_TO_JSON(stencilFrontFail);
			ADD_TO_JSON(stencilFrontFunc);
			ADD_TO_JSON(stencilFrontPass);
			ADD_TO_JSON(stencilFrontZFail);

#if DEBUG
			stateBitEntry.AddMember("check0", entry.flags.loadbit0, allocator);
			stateBitEntry.AddMember("check1", entry.flags.loadbit1, allocator);
#endif

			arr.PushBack(stateBitEntry, allocator);
		}

		return arr;
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

		// ???
		mat.gameFlags.fields.unk8 = material->info.gameFlags.fields.unk7;
		mat.gameFlags.fields.unk7 = material->info.gameFlags.fields.unk8;

		// Do not delay model surface ever! In iw4 this doesn't even exist
		// If this flag is ever set to 1 it usually burns the delayed surface buffer of IW4
		mat.gameFlags.fields.delayModelSurface = 0; 

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

		mat.techniqueSet   = material->techniqueSet;
		mat.textureTable   = material->textureTable;
		mat.constantTable  = material->constantTable;
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

		IMaterial::SaveConvertedMaterial(&mat);
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
