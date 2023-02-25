#include "STDInclude.hpp"

#define IW4X_TECHSET_VERSION 1

namespace Components
{
	const std::string IMaterialTechniqueSet::techsetSuffix = ".3x";

	const char* IMaterialTechniqueSet::declStreamDestinations[]
	{
		"p",
		"n",
		"c0",
		"c1",
		"d",
		"t0",
		"t1",
		"t2",
		"t3",
		"t4",
		"t5",
		"t6",
		"t7",
	};

	const char* IMaterialTechniqueSet::declStreamSources[]
	{
		"p",
		"c",
		"t0",
		"n",
		"t",
		"t1",
		"t2",
		"n0",
		"n1"
	};

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

		// missing - unused ?
		//{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SHADOW_PARMS, Game::IW4::ShaderCodeConstants::CONST_SRC_NONE },

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

		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SUN_POSITION, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_POSITION },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SUN_DIFFUSE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_DIFFUSE },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SUN_SPECULAR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_SPECULAR },

		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DEBUG_BUMPMAP, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DEBUG_BUMPMAP },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_MATERIAL_COLOR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_MATERIAL_COLOR },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FOG, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FOG },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR_GAMMA }, // this is CORRECT! Do not map me to LINEAR_COLOR ! GAMMA is in fact gamma COLOR which is what IW3 has !
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
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_DEPTH_FROM_CLIP, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_DEPTH_FROM_CLIP },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_0, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_1, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_1 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_LAST, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_CODE_MESH_ARG_LAST },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_BASE_LIGHTING_COORDS, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_BASE_LIGHTING_COORDS },

		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_VIEW_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_VIEW_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX },

		// Matrix => Matrix0
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 },
		{ Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 },

		{ Game::IW3::ShaderCodeConstants::CONST_SRC_NONE, Game::IW4::ShaderCodeConstants::CONST_SRC_NONE },
	};

	const std::unordered_map<Game::IW4::MaterialTechniqueType, Game::IW3::MaterialTechniqueType> IMaterialTechniqueSet::techniqueTypeTableFromIW4 = {
		{ Game::IW4::TECHNIQUE_DEPTH_PREPASS, Game::IW3::TECHNIQUE_DEPTH_PREPASS},
		{ Game::IW4::TECHNIQUE_BUILD_FLOAT_Z, Game::IW3::TECHNIQUE_BUILD_FLOAT_Z},
		{ Game::IW4::TECHNIQUE_BUILD_SHADOWMAP_DEPTH, Game::IW3::TECHNIQUE_BUILD_SHADOWMAP_DEPTH},
		{ Game::IW4::TECHNIQUE_BUILD_SHADOWMAP_COLOR, Game::IW3::TECHNIQUE_BUILD_SHADOWMAP_COLOR},
		{ Game::IW4::TECHNIQUE_UNLIT, Game::IW3::TECHNIQUE_UNLIT},
		{ Game::IW4::TECHNIQUE_EMISSIVE, Game::IW3::TECHNIQUE_EMISSIVE},
		{ Game::IW4::TECHNIQUE_EMISSIVE_SHADOW, Game::IW3::TECHNIQUE_EMISSIVE_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT, Game::IW3::TECHNIQUE_LIT},
		{ Game::IW4::TECHNIQUE_LIT_SUN, Game::IW3::TECHNIQUE_LIT_SUN},
		{ Game::IW4::TECHNIQUE_LIT_SUN_SHADOW, Game::IW3::TECHNIQUE_LIT_SUN_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_SPOT, Game::IW3::TECHNIQUE_LIT_SPOT},
		{ Game::IW4::TECHNIQUE_LIT_SPOT_SHADOW, Game::IW3::TECHNIQUE_LIT_SPOT_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_OMNI, Game::IW3::TECHNIQUE_LIT_OMNI},
		{ Game::IW4::TECHNIQUE_LIT_OMNI_SHADOW, Game::IW3::TECHNIQUE_LIT_OMNI_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED, Game::IW3::TECHNIQUE_LIT_INSTANCED},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN, Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW, Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT, Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW, Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI, Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW, Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW},

		{ Game::IW4::TECHNIQUE_LIGHT_SPOT, Game::IW3::TECHNIQUE_LIGHT_SPOT},
		{ Game::IW4::TECHNIQUE_LIGHT_OMNI, Game::IW3::TECHNIQUE_LIGHT_OMNI},
		{ Game::IW4::TECHNIQUE_LIGHT_SPOT_SHADOW, Game::IW3::TECHNIQUE_LIGHT_SPOT_SHADOW},
		{ Game::IW4::TECHNIQUE_FAKELIGHT_NORMAL, Game::IW3::TECHNIQUE_FAKELIGHT_NORMAL},
		{ Game::IW4::TECHNIQUE_FAKELIGHT_VIEW, Game::IW3::TECHNIQUE_FAKELIGHT_VIEW},
		{ Game::IW4::TECHNIQUE_SUNLIGHT_PREVIEW, Game::IW3::TECHNIQUE_SUNLIGHT_PREVIEW},
		{ Game::IW4::TECHNIQUE_CASE_TEXTURE, Game::IW3::TECHNIQUE_CASE_TEXTURE},
		{ Game::IW4::TECHNIQUE_WIREFRAME_SOLID, Game::IW3::TECHNIQUE_WIREFRAME_SOLID},
		{ Game::IW4::TECHNIQUE_WIREFRAME_SHADED, Game::IW3::TECHNIQUE_WIREFRAME_SHADED},
		{ Game::IW4::TECHNIQUE_DEBUG_BUMPMAP, Game::IW3::TECHNIQUE_DEBUG_BUMPMAP},
		{ Game::IW4::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED, Game::IW3::TECHNIQUE_DEBUG_BUMPMAP_INSTANCED},

		// Wild guesses :
		{ Game::IW4::TECHNIQUE_EMISSIVE_DFOG, Game::IW3::TECHNIQUE_EMISSIVE},
		{ Game::IW4::TECHNIQUE_EMISSIVE_SHADOW_DFOG, Game::IW3::TECHNIQUE_EMISSIVE_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_DFOG, Game::IW3::TECHNIQUE_LIT},
		{ Game::IW4::TECHNIQUE_LIT_SUN_DFOG, Game::IW3::TECHNIQUE_LIT_SUN},
		{ Game::IW4::TECHNIQUE_LIT_SUN_SHADOW_DFOG, Game::IW3::TECHNIQUE_LIT_SUN_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_SPOT_DFOG, Game::IW3::TECHNIQUE_LIT_SPOT},
		{ Game::IW4::TECHNIQUE_LIT_SPOT_SHADOW_DFOG, Game::IW3::TECHNIQUE_LIT_SPOT_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_OMNI_DFOG, Game::IW3::TECHNIQUE_LIT_OMNI},
		{ Game::IW4::TECHNIQUE_LIT_OMNI_SHADOW_DFOG, Game::IW3::TECHNIQUE_LIT_OMNI_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_DFOG, Game::IW3::TECHNIQUE_LIT_INSTANCED},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_DFOG, Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG, Game::IW3::TECHNIQUE_LIT_INSTANCED_SUN_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_DFOG, Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG, Game::IW3::TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_DFOG, Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI},
		{ Game::IW4::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG, Game::IW3::TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW},
	};

	const std::unordered_map<Game::IW3::MaterialTextureSource, Game::IW4::MaterialTextureSource> IMaterialTechniqueSet::samplerTableToIW4 =
	{
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_BLACK, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_BLACK },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_WHITE , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_WHITE },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_MODEL_LIGHTING , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_MODEL_LIGHTING },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY,Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY,Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY },


		  //{ Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWCOOKIE, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_WHITE },       // incorrect!

		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWMAP_SUN, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWMAP_SUN },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWMAP_SPOT , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWMAP_SPOT },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_FEEDBACK , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_FEEDBACK },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_RESOLVED_POST_SUN ,Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_RESOLVED_POST_SUN },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_RESOLVED_SCENE , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_RESOLVED_SCENE},
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_POST_EFFECT_0 , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_POST_EFFECT_0},
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_POST_EFFECT_1 , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_POST_EFFECT_1},

		  // Unused!
		  //{ Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_SKY , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_BLACK },

		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_LIGHT_ATTENUATION , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_LIGHT_ATTENUATION},

		  // Unused!
		  //{ Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_DYNAMIC_SHADOWS , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_SHADOWMAP_SPOT },

		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_OUTDOOR , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_OUTDOOR },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_FLOATZ , Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_FLOATZ },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_PROCESSED_FLOATZ, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_PROCESSED_FLOATZ },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_RAW_FLOATZ, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_RAW_FLOATZ },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_CASE_TEXTURE, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_CASE_TEXTURE },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_Y, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_Y },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_CR, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_CR },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_CB, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_CB},
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_A, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_CINEMATIC_A },
		  { Game::IW3::MaterialTextureSource::TEXTURE_SRC_CODE_REFLECTION_PROBE, Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_REFLECTION_PROBE }
	};

	Game::IW3::MaterialTechnique* IMaterialTechniqueSet::ConvertTechnique(Game::IW3::MaterialTechnique* tech)
	{
		AssertSize(Game::IW3::MaterialPass, 20);
		if (!tech) return nullptr;

		const auto size =
			sizeof Game::IW3::MaterialTechnique
			- sizeof Game::IW3::MaterialPass
			+ sizeof Game::IW3::MaterialPass * tech->passCount;

		auto iw4Technique = reinterpret_cast<Game::IW3::MaterialTechnique*>(
			LocalAllocator.Allocate(
				size
			)
			);

		std::memcpy(iw4Technique, tech, size);
		iw4Technique->name = LocalAllocator.DuplicateString(std::format("{}{}", tech->name, techsetSuffix));

		// We complete these later
		iw4Technique->flags = tech->flags << 2;

		for (int i = 0; i < tech->passCount; i++)
		{
			const Game::IW3::MaterialPass* pass = &tech->passArray[i];
			auto iw4Pass = &iw4Technique->passArray[i];

			bool isWaterPass = false;
			constexpr auto hashToLookFor = Utils::HashString("waterColor");

			// Detect if this is about water!
			for (int k = 0; k < pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount; ++k)
			{
				Game::IW3::MaterialShaderArgument* arg = &pass->args[k];

				if (arg->type == Game::MTL_ARG_MATERIAL_PIXEL_CONST)
				{
					if (arg->u.nameHash == hashToLookFor)
					{
						isWaterPass = true;
						break;
					}
				}
			}

			if (pass->vertexDecl)
			{
				// Wrong but not important
				iw4Pass->vertexDecl = reinterpret_cast<Game::IW3::MaterialVertexDeclaration*>(IMaterialTechniqueSet::ConvertDecl(pass->vertexDecl));
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_VERTEXDECL, iw4Pass->vertexDecl);

			}

			if (pass->vertexShader)
			{
				iw4Pass->vertexShader = IMaterialTechniqueSet::ConvertVS(pass->vertexShader);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_VERTEXSHADER, iw4Pass->vertexShader);

			}

			if (pass->pixelShader)
			{
				iw4Pass->pixelShader = IMaterialTechniqueSet::ConvertPS(pass->pixelShader);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_PIXELSHADER, iw4Pass->pixelShader);
			}

			iw4Pass->args = LocalAllocator.AllocateArray<Game::IW3::MaterialShaderArgument>(pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount);
			for (int k = 0; k < pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount; ++k)
			{
				Game::IW3::MaterialShaderArgument* arg = &pass->args[k];
				auto* iw4Arg = &iw4Pass->args[k];

				std::memcpy(iw4Arg, arg, sizeof(Game::IW3::MaterialShaderArgument));

				if (arg->type == Game::MTL_ARG_LITERAL_PIXEL_CONST || arg->type == Game::MTL_ARG_LITERAL_VERTEX_CONST)
				{
					iw4Arg->u.literalConst = LocalAllocator.AllocateArray<float>(4);
					std::memcpy(iw4Arg->u.literalConst, arg->u.literalConst, 4 * sizeof(float));
				}
				else if (arg->type == Game::MaterialShaderArgumentType::MTL_ARG_CODE_VERTEX_CONST
					|| arg->type == Game::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_CONST)
				{
					auto newIndex = IMaterialTechniqueSet::iw3CodeConstMap.find(arg->u.codeConst.index);
					if (newIndex == IMaterialTechniqueSet::iw3CodeConstMap.end())
					{
						Logger::Print("Unable to map code constant %d for technique '%s'! Not exporting technique\n", arg->u.codeConst.index, tech->name);
						return nullptr;
					}

					unsigned short val = (unsigned short)newIndex->second;

					if (isWaterPass && val == Game::IW3::ShaderCodeConstants::CONST_SRC_CODE_LIGHT_POSITION)
					{
						// If IsWaterTechnique => fuck up the sun position
						// Because it's not correct for iw3 anyway and so unexpected
						Logger::Print("Routed the pass argument %i to the wrong constant %i instead of %i on purpose because it is a water material!\n", k, Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR_LINEAR, val);
						val = Game::IW4::ShaderCodeConstants::CONST_SRC_CODE_FOG_COLOR_LINEAR; // Anything normalized & stable will do
					}

					iw4Arg->u.codeConst.index = val;
				}
				else if (arg->type == Game::MaterialShaderArgumentType::MTL_ARG_CODE_PIXEL_SAMPLER)
				{
					Game::IW4::MaterialTextureSource codeSampler{};

					if (!samplerTableToIW4.contains(static_cast<Game::IW3::MaterialTextureSource>(arg->u.codeSampler)))
					{
						Logger::Print("Unmapped sampler %i! This normally never happens, or should be very unusual! Mapping to black.\n");
						codeSampler = Game::IW4::MaterialTextureSource::TEXTURE_SRC_CODE_BLACK;
					}
					else
					{
						codeSampler = samplerTableToIW4.at(static_cast<Game::IW3::MaterialTextureSource>(arg->u.codeSampler));

#define MTL_TECHFLAG_NEEDS_RESOLVED_POST_SUN 1
#define MTL_TECHFLAG_NEEDS_RESOLVED_SCENE 2

						if (codeSampler == Game::IW4::TEXTURE_SRC_CODE_RESOLVED_POST_SUN)
						{
							iw4Technique->flags |= MTL_TECHFLAG_NEEDS_RESOLVED_POST_SUN;
						}

						if (codeSampler == Game::IW4::TEXTURE_SRC_CODE_RESOLVED_SCENE)
						{
							iw4Technique->flags |= MTL_TECHFLAG_NEEDS_RESOLVED_SCENE;
						}
					}

					iw4Arg->u.codeSampler = codeSampler;
				}
			}
		}

		return iw4Technique;
	}

	Game::IW4::MaterialVertexDeclaration* IMaterialTechniqueSet::ConvertDecl(Game::IW3::MaterialVertexDeclaration* decl)
	{
		if (!decl) return nullptr;

		Game::IW4::MaterialVertexDeclaration* iw4Decl = LocalAllocator.Allocate<Game::IW4::MaterialVertexDeclaration>();

		iw4Decl->name = LocalAllocator.DuplicateString(GetDeclarationName(decl));
		iw4Decl->hasOptionalSource = decl->hasOptionalSource;
		iw4Decl->streamCount = decl->streamCount;

		// Is this related to GfxRenderTargetId ??
		// iw3 has 16
		// iw4 has 13
		// Just like the routing info ! 
		// Probably unrelated but interesting to note

		memcpy(&iw4Decl->routing, &decl->routing, sizeof(Game::IW4::MaterialVertexStreamRouting));

		for (int i = 0; i < iw4Decl->streamCount; i++)
		{
			// add STREAM_DST_DEPTH destination
			if (iw4Decl->routing.data[i].dest >= 4)
			{
				iw4Decl->routing.data[i].dest =
					static_cast<Game::IW4::MaterialStreamRoutingDestination>(
						static_cast<char>(iw4Decl->routing.data[i].dest) + 1
						);
			}

			if (iw4Decl->routing.data[i].dest >= Game::IW4::MaterialStreamRoutingDestination::STREAM_DST_COUNT
				|| iw4Decl->routing.data[i].source >= Game::IW4::MaterialStreamRoutingSource::STREAM_SRC_COUNT)
			{
				Logger::Print("Warning: routing data is wrong (%d, %d)\n", iw4Decl->routing.data[i].source, iw4Decl->routing.data[i].dest);
			}
		}

		return iw4Decl;
	}

	Game::IW3::MaterialVertexShader* IMaterialTechniqueSet::ConvertVS(Game::IW3::MaterialVertexShader* vs)
	{
		if (!vs) return vs;

		if (vs->prog.loadDef.loadForRenderer == Game::IW3::GfxRenderer::GFX_RENDERER_SHADER_2)
		{
			Logger::Print("Not exporting vertex shader %s because they're targeting a SM2 renderer which iw4 does not have!", vs->name);
			return nullptr;
		}

		auto iw4Vs = LocalAllocator.Allocate<Game::IW3::MaterialVertexShader>();
		iw4Vs->name = LocalAllocator.DuplicateString(std::format("{}{}", vs->name, IMaterialTechniqueSet::techsetSuffix));
		iw4Vs->prog.loadDef.loadForRenderer = Game::IW3::GfxRenderer::GFX_RENDERER_SHADER_3;
		iw4Vs->prog.loadDef.program = LocalAllocator.AllocateArray<unsigned int>(vs->prog.loadDef.programSize);
		iw4Vs->prog.loadDef.programSize = vs->prog.loadDef.programSize;

		std::memcpy(iw4Vs->prog.loadDef.program, vs->prog.loadDef.program, vs->prog.loadDef.programSize * sizeof(unsigned int));

		return iw4Vs;
	}

	Game::IW3::MaterialPixelShader* IMaterialTechniqueSet::ConvertPS(Game::IW3::MaterialPixelShader* ps)
	{
		if (!ps) return ps;

		if (ps->prog.loadDef.loadForRenderer == Game::IW3::GfxRenderer::GFX_RENDERER_SHADER_2)
		{
			Logger::Print("Not exporting pixel shader %s because they're targeting a SM2 renderer which iw4 does not have!", ps->name);
			return nullptr;
		}

		auto iw4Ps = LocalAllocator.Allocate<Game::IW3::MaterialPixelShader>();
		iw4Ps->name = LocalAllocator.DuplicateString(std::format("{}{}", ps->name, IMaterialTechniqueSet::techsetSuffix));
		iw4Ps->prog.loadDef.loadForRenderer = Game::IW3::GfxRenderer::GFX_RENDERER_SHADER_3;
		iw4Ps->prog.loadDef.program = LocalAllocator.AllocateArray<unsigned int>(ps->prog.loadDef.programSize);
		iw4Ps->prog.loadDef.programSize = ps->prog.loadDef.programSize;

		std::memcpy(iw4Ps->prog.loadDef.program, ps->prog.loadDef.program, ps->prog.loadDef.programSize * sizeof(unsigned int));

		return iw4Ps;
	}

	Game::IW4::MaterialTechniqueSet* IMaterialTechniqueSet::Convert(Game::IW3::MaterialTechniqueSet* techset)
	{
		if (!techset) return nullptr;

		Game::IW4::MaterialTechniqueSet* iw4Techset = LocalAllocator.Allocate<Game::IW4::MaterialTechniqueSet>();

		auto name = std::format("{}{}", techset->name, techsetSuffix);
		iw4Techset->name = LocalAllocator.DuplicateString(name);


		iw4Techset->worldVertFormat = techset->worldVertFormat; // enum didn't change
		iw4Techset->hasBeenUploaded = techset->hasBeenUploaded;
		iw4Techset->unused[0] = techset->unused[0]; // ?

		static std::regex zFeatherRegx = std::regex("_zfeather");
		static std::regex smRegx = std::regex("_sm");

		if (name.contains("_zfeather"))
		{
			techset->remappedTechniqueSet = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_TECHNIQUE_SET, std::regex_replace(techset->name, zFeatherRegx, "").data()).techniqueSet;
			iw4Techset->remappedTechniqueSet = Convert(techset->remappedTechniqueSet);
		}
		else if (name.contains("_sm"))
		{
			techset->remappedTechniqueSet = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_TECHNIQUE_SET, std::regex_replace(techset->name, smRegx, "_hsm").data()).techniqueSet;
			iw4Techset->remappedTechniqueSet = Convert(techset->remappedTechniqueSet);
		}

		// copy techniques to correct spots
		for (size_t i = 0; i < Game::IW4::TECHNIQUE_COUNT; i++)
		{
			Game::IW4::MaterialTechniqueType technique = static_cast<Game::IW4::MaterialTechniqueType>(i);
			if (techniqueTypeTableFromIW4.contains(technique))
			{
				iw4Techset->techniques[technique] = ConvertTechnique(techset->techniques[techniqueTypeTableFromIW4.at(technique)]);
			}
			else
			{
				iw4Techset->techniques[technique] = nullptr;
			}
		}

		if (iw4Techset->remappedTechniqueSet && 
			iw4Techset->remappedTechniqueSet->remappedTechniqueSet &&
			iw4Techset->remappedTechniqueSet->remappedTechniqueSet == iw4Techset)
		{
			assert(false);
		}

		return iw4Techset;
	}

	std::string IMaterialTechniqueSet::GetDeclarationName(const Game::IW3::MaterialVertexDeclaration* decl) {
		std::ostringstream output;

		for (auto i = 0; i < decl->streamCount; i++)
		{
			const auto& stream = decl->routing.data[i];

			if (stream.dest >= Game::IW3::MaterialStreamRoutingDestination::STREAM_DST_COUNT)
			{
				Logger::Print("Material routing error! Dest was %i, unexpectedly high\n", stream.dest);
			}
			else if (stream.source >= Game::IW3::MaterialStreamRoutingSource::STREAM_SRC_COUNT)
			{
				Logger::Print("Material routing error! Src was %i, unexpectedly high (max is %i)\n");
			}
			else
			{
				output << declStreamSources[stream.source] << declStreamDestinations[stream.dest];
			}
		}

		if (output.str().size() == 0) {
			static int numDecls = 0;
			return Utils::VA("iw4xDecl%d", ++numDecls);;
		}

		return output.str();
	}

	IMaterialTechniqueSet::IMaterialTechniqueSet()
	{
		Command::Add("dumpTechset", [](const Command::Params& params)
			{
				if (params.Length() < 2) return;
				auto converted = IMaterialTechniqueSet::Convert(Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_TECHNIQUE_SET, params[1]).techniqueSet);
				MapDumper::GetApi()->write(Game::IW4::XAssetType::ASSET_TYPE_TECHNIQUE_SET, converted);
			});
	}

	IMaterialTechniqueSet::~IMaterialTechniqueSet()
	{

	}
}
