#include "STDInclude.hpp"

#define IW4X_TECHSET_VERSION '0'

namespace Components
{

    void IMaterialTechniqueSet::DumpTechnique(Game::IW3::MaterialTechnique* tech)
    {
        AssertSize(Game::IW3::MaterialPass, 20);
        if (!tech) return;
        Utils::Stream buffer;
        buffer.saveArray("IW4xTECH", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(tech->flags);
        buffer.saveObject(tech->passCount);

        if (tech->passCount == 0)
        {
            OutputDebugStringA("");
        }

        buffer.saveArray(tech->passArray, tech->passCount);

        for (int i = 0; i < tech->passCount; i++)
        {
            Game::IW3::MaterialPass* pass = &tech->passArray[i];

            if (pass->vertexDecl)
            {
                std::string name = IMaterialTechniqueSet::DumpDecl(pass->vertexDecl);
                buffer.saveString(name.data());
            }

            if (pass->vertexShader)
            {
                buffer.saveString(pass->vertexShader->name);
                IMaterialTechniqueSet::DumpVS(pass->vertexShader);
            }

            if (pass->pixelShader)
            {
                buffer.saveString(pass->pixelShader->name);
                IMaterialTechniqueSet::DumpPS(pass->pixelShader);
            }

            buffer.saveArray(pass->args, pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount);

            for (int k = 0; k < pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount; ++k)
            {
                Game::IW3::MaterialShaderArgument* arg = &pass->args[k];
                if (arg->type == MTL_ARG_LITERAL_VERTEX_CONST || arg->type == MTL_ARG_LITERAL_PIXEL_CONST)
                {
                    buffer.saveArray(arg->u.literalConst, 4);
                }

                if (arg->type == MTL_ARG_CODE_VERTEX_CONST || arg->type == MTL_ARG_CODE_PIXEL_CONST)
                {
                    auto newIndex = IMaterialTechniqueSet::iw3CodeConstMap.find(arg->u.codeConst.index);
                    if (newIndex == IMaterialTechniqueSet::iw3CodeConstMap.end())
                    {
                        Logger::Print("Unable to map code constant %d for technique '%s'! Not exporting technique\n", arg->u.codeConst.index, tech->name);
                        return;
                    }
                    unsigned short val = (unsigned short)newIndex->second;
                    buffer.saveObject(val);
                    buffer.saveObject(arg->u.codeConst.firstRow);
                    buffer.saveObject(arg->u.codeConst.rowCount);
                }
            }
        }

        Utils::WriteFile(Utils::VA("%s/techniques/%s.iw4xTech", AssetHandler::GetExportPath().data(), tech->name), buffer.toBuffer());
    }

    std::string IMaterialTechniqueSet::DumpDecl(Game::IW3::MaterialVertexDeclaration* decl)
    {
        if (!decl) return "";

        static int numDecls = 0;
        Utils::Memory::Allocator allocator;

        Game::IW4::MaterialVertexDeclaration* iw4Decl = allocator.allocate<Game::IW4::MaterialVertexDeclaration>();

        // TODO: figure out how to actually name these things
        iw4Decl->name = allocator.duplicateString(Utils::VA("iw4xDecl%d", numDecls++));
        iw4Decl->hasOptionalSource = decl->hasOptionalSource;
        iw4Decl->streamCount = decl->streamCount;

        memcpy(&iw4Decl->routing, &decl->routing, sizeof(Game::IW4::MaterialVertexStreamRouting));

        for (int i = 0; i < iw4Decl->streamCount; i++)
        {
            // add in depth destination
            if (iw4Decl->routing.data[i].dest >= 4)
            {
                iw4Decl->routing.data[i].dest += 1;
            }

            if (iw4Decl->routing.data[i].dest > 12 || iw4Decl->routing.data[i].source > 8)
            {
                Logger::Print("Warning: routing data is wrong (%d, %d)\n", iw4Decl->routing.data[i].source, iw4Decl->routing.data[i].dest);
            }
        }

        Utils::Stream buffer;
        buffer.saveArray("IW4xDECL", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*iw4Decl);

        if (iw4Decl->name)
        {
            buffer.saveString(iw4Decl->name);
        }

        Utils::WriteFile(Utils::VA("%s/decl/%s.iw4xDECL", AssetHandler::GetExportPath().data(), iw4Decl->name), buffer.toBuffer());

        return iw4Decl->name;
    }

    void IMaterialTechniqueSet::DumpVS(Game::IW3::MaterialVertexShader* vs)
    {
        if (!vs) return;
        Utils::Stream buffer;
        buffer.saveArray("IW4xVERT", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*vs);

        if (vs->name)
        {
            buffer.saveString(vs->name);
        }

        buffer.saveArray(vs->prog.loadDef.program, vs->prog.loadDef.programSize);

        Utils::WriteFile(Utils::VA("%s/vs/%s.iw4xVS", AssetHandler::GetExportPath().data(), vs->name), buffer.toBuffer());
    }

    void IMaterialTechniqueSet::DumpPS(Game::IW3::MaterialPixelShader* ps)
    {
        if (!ps) return;
        Utils::Stream buffer;
        buffer.saveArray("IW4xPIXL", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*ps);

        if (ps->name)
        {
            buffer.saveString(ps->name);
        }

        buffer.saveArray(ps->prog.loadDef.program, ps->prog.loadDef.programSize);

        Utils::WriteFile(Utils::VA("%s/ps/%s.iw4xPS", AssetHandler::GetExportPath().data(), ps->name), buffer.toBuffer());
    }

    void IMaterialTechniqueSet::Dump(Game::IW3::MaterialTechniqueSet* techset)
    {
        if (!techset) return;

        static int numDecls = 0;
        Utils::Memory::Allocator allocator;

        Game::IW4::MaterialTechniqueSet* iw4Techset = allocator.allocate<Game::IW4::MaterialTechniqueSet>();

        iw4Techset->name = techset->name;
        iw4Techset->worldVertFormat = techset->worldVertFormat; // enum didn't change
        iw4Techset->hasBeenUploaded = false;
        iw4Techset->remappedTechniqueSet = nullptr;

        // copy techniques to correct spots
        iw4Techset->techniques[Game::IW4::TECHNIQUE_DEPTH_PREPASS] = techset->techniques[Game::IW3::TECHNIQUE_DEPTH_PREPASS];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_BUILD_FLOAT_Z] = techset->techniques[Game::IW3::TECHNIQUE_BUILD_FLOAT_Z];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_BUILD_SHADOWMAP_DEPTH] = techset->techniques[Game::IW3::TECHNIQUE_BUILD_SHADOWMAP_DEPTH];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_BUILD_SHADOWMAP_COLOR] = techset->techniques[Game::IW3::TECHNIQUE_BUILD_SHADOWMAP_COLOR];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_UNLIT] = techset->techniques[Game::IW3::TECHNIQUE_UNLIT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_EMISSIVE] = techset->techniques[Game::IW3::TECHNIQUE_EMISSIVE];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_EMISSIVE_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_EMISSIVE];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_EMISSIVE_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_EMISSIVE_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_EMISSIVE_SHADOW_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_EMISSIVE_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_BEGIN] = techset->techniques[Game::IW3::TECHNIQUE_LIT_BEGIN];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT] = techset->techniques[Game::IW3::TECHNIQUE_LIT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SUN] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SUN];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SUN_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SUN];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SUN_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SUN_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SUN_SHADOW_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SUN_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SPOT] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SPOT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SPOT_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SPOT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SPOT_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SPOT_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_SPOT_SHADOW_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_SPOT_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_OMNI] = techset->techniques[Game::IW3::TECHNIQUE_LIT_OMNI];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_OMNI_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_OMNI];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_OMNI_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_LIT_OMNI_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_OMNI_SHADOW_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_OMNI_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG] = techset->techniques[Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIT_END] = techset->techniques[Game::IW3::TECHNIQUE_LIT_END];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIGHT_SPOT] = techset->techniques[Game::IW3::TECHNIQUE_LIGHT_SPOT];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIGHT_OMNI] = techset->techniques[Game::IW3::TECHNIQUE_LIGHT_OMNI];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_LIGHT_SPOT_SHADOW] = techset->techniques[Game::IW3::TECHNIQUE_LIGHT_SPOT_SHADOW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_FAKELIGHT_NORMAL] = techset->techniques[Game::IW3::TECHNIQUE_FAKELIGHT_NORMAL];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_FAKELIGHT_VIEW] = techset->techniques[Game::IW3::TECHNIQUE_FAKELIGHT_VIEW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_SUNLIGHT_PREVIEW] = techset->techniques[Game::IW3::TECHNIQUE_SUNLIGHT_PREVIEW];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_CASE_TEXTURE] = techset->techniques[Game::IW3::TECHNIQUE_CASE_TEXTURE];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_WIREFRAME_SOLID] = techset->techniques[Game::IW3::TECHNIQUE_WIREFRAME_SOLID];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_WIREFRAME_SHADED] = techset->techniques[Game::IW3::TECHNIQUE_WIREFRAME_SHADED];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_DEBUG_BUMPMAP] = techset->techniques[Game::IW3::TECHNIQUE_DEBUG_BUMPMAP];
        iw4Techset->techniques[Game::IW4::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED] = techset->techniques[Game::IW3::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED];

        Utils::Stream buffer;
        buffer.saveArray("IW4xTSET", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*iw4Techset);

        if (iw4Techset->name)
        {
            buffer.saveString(iw4Techset->name);
        }

        for (int i = 0; i < 48; i++)
        {
            if (iw4Techset->techniques[i])
            {
                buffer.saveString(iw4Techset->techniques[i]->name);
                IMaterialTechniqueSet::DumpTechnique(iw4Techset->techniques[i]);
            }
        }

        Utils::WriteFile(Utils::VA("%s/techsets/%s.iw4xTS", AssetHandler::GetExportPath().data(), iw4Techset->name), buffer.toBuffer());
    }

    IMaterialTechniqueSet::IMaterialTechniqueSet()
    {
        Command::Add("dumpTechset", [](Command::Params params)
        {
            if (params.Length() < 2) return;
            IMaterialTechniqueSet::Dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET, params[1]).techniqueSet);
        });
    }

    IMaterialTechniqueSet::~IMaterialTechniqueSet()
    {

    }

    std::unordered_map <std::int32_t, std::int32_t> IMaterialTechniqueSet::iw3CodeConstMap =
    {
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_POSITION, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_POSITION },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_DIFFUSE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_DIFFUSE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_SPECULAR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_SPECULAR },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_SPOTDIR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_SPOTDIR },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_SPOTFACTORS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_SPOTFACTORS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_NEARPLANE_ORG, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_NEARPLANE_ORG },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_NEARPLANE_DX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_NEARPLANE_DX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_NEARPLANE_DY, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_NEARPLANE_DY },
        // missing
        // { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SHADOW_PARMS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SHADOW_PARAMS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_RENDER_TARGET_SIZE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_RENDER_TARGET_SIZE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DOF_EQUATION_SCENE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DOF_EQUATION_SCENE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DOF_LERP_SCALE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DOF_LERP_SCALE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DOF_LERP_BIAS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DOF_LERP_BIAS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DOF_ROW_DELTA, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DOF_ROW_DELTA },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_PARTICLE_CLOUD_COLOR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_PARTICLE_CLOUD_COLOR },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_GAMETIME, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_GAMETIME },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_PIXEL_COST_FRACS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_PIXEL_COST_FRACS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_PIXEL_COST_DECODE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_PIXEL_COST_DECODE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_0, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_1, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_1 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_2, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_2 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_3, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_3 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_4, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_4 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_5, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_5 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_6, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_6 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_7, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FILTER_TAP_7 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_COLOR_MATRIX_R, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_COLOR_MATRIX_R },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_COLOR_MATRIX_G, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_COLOR_MATRIX_G },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_COLOR_MATRIX_B, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_COLOR_MATRIX_B },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SHADOWMAP_SCALE , Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SHADOWMAP_SCALE  },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_ZNEAR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_ZNEAR },
        // missing
        //{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SUN_POSITION, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SUN_POSITION },
        //{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SUN_DIFFUSE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SUN_DIFFUSE },
        //{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SUN_SPECULAR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SUN_SPECULAR },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DEBUG_BUMPMAP, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DEBUG_BUMPMAP },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_MATERIAL_COLOR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_MATERIAL_COLOR },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FOG, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FOG },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR_LINEAR },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_GLOW_SETUP, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_GLOW_SETUP },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_GLOW_APPLY, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_GLOW_APPLY },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_COLOR_BIAS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_COLOR_BIAS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_COLOR_TINT_BASE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_COLOR_TINT_BASE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_COLOR_TINT_DELTA, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_COLOR_TINT_DELTA },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_ENVMAP_PARMS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_ENVMAP_PARMS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DEPTH_FROM_CLIP, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DEPTH_FROM_CLIP },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_0, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_1, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_1 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_LAST, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_LAST },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_BASE_LIGHTING_COORDS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_BASE_LIGHTING_COORDS },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_COUNT_FLOAT4, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_COUNT_FLOAT4 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_FIRST_CODE_MATRIX , Game::IW4::ShaderCodeConstants::CONST_SRC_FIRST_CODE_MATRIX  },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_VIEW_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_VIEW_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_TOTAL_COUNT, Game::IW4::ShaderCodeConstants::CONST_SRC_TOTAL_COUNT },
        { Game::IW3::ShaderCodeConstants::CONST_SRC_NONE, Game::IW4::ShaderCodeConstants::CONST_SRC_NONE },
    };

}
