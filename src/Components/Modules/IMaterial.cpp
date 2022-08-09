#include "STDInclude.hpp"

#define IW4X_MAT_VERSION 1

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
							auto ptr = reinterpret_cast<uint8_t*>(&water->H0);
							auto buffer = std::vector<uint8_t>(ptr, ptr + water->M * water->N * sizeof(Game::IW3::complex_s));

							waterJson.AddMember("H0", RAPIDJSON_STR(strDuplicator.duplicateString(Utils::Base64::Encode(buffer))), allocator);
						}

						if (water->wTerm)
						{
							auto ptr = reinterpret_cast<uint8_t*>(&water->wTerm);
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


		mat.gameFlags.fields.unk8 = material->info.gameFlags.fields.unk7;
		mat.gameFlags.fields.unk7 = material->info.gameFlags.fields.unk8;
			
		mat.sortKey                 = GetConvertedSortKey(material);
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
		mat.cameraRegion   = material->cameraRegion;

		if (mat.cameraRegion == 0x3) {
			// 0x3 is NONE in iw3, but DEPTH_HACK in iw4
			// In iw4 NONE is 0x4
			Logger::Print(Utils::VA("Swapped material %s camera region from 0x3 to 0x4 (NONE)\n", mat.name));
			mat.cameraRegion = 0x4;
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

		const std::string& name = material->info.name;

		if (material->techniqueSet->name == "2d"s)
		{
			//"blend / additive" => SORTKEY_BLEND_ADDITIVE
			return 47;
		}

		if (iw3Key == 4) 
		{
			if (std::string(material->techniqueSet->name).contains("ambient"s))
			{
				// Opaque => Opaque_Ambient
				return 0;
			}
		}

		// decal - static decal got broken down in multiple categorries
		if (iw3Key == 12) 
		{
			if (std::string(material->techniqueSet->name).contains("shadow"s))
			{
				return 34;
			}

		}



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
