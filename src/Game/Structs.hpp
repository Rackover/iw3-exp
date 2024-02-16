#pragma once

// This allows us to compile our structures in IDA, for easier reversing :3
#ifdef __cplusplus
namespace Game
{
#else
#define COD4
#define PC
#endif

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

typedef unsigned __int16 ScriptString;

enum TextureSemantic : char
{
	TS_2D = 0x0,
	TS_FUNCTION = 0x1,
	TS_COLOR_MAP = 0x2,
	TS_DETAIL_MAP = 0x3,
	TS_UNUSED_2 = 0x4,
	TS_NORMAL_MAP = 0x5,
	TS_UNUSED_3 = 0x6,
	TS_UNUSED_4 = 0x7,
	TS_SPECULAR_MAP = 0x8,
	TS_UNUSED_5 = 0x9,
	TS_UNUSED_6 = 0xA,
	TS_WATER_MAP = 0xB,
};

enum GfxImageCategory : char
{
	IMG_CATEGORY_UNKNOWN = 0x0,
	IMG_CATEGORY_AUTO_GENERATED = 0x1,
	IMG_CATEGORY_LIGHTMAP = 0x2,
	IMG_CATEGORY_LOAD_FROM_FILE = 0x3,
	IMG_CATEGORY_RAW = 0x4,
	IMG_CATEGORY_FIRST_UNMANAGED = 0x5,
	IMG_CATEGORY_WATER = 0x5,
	IMG_CATEGORY_RENDERTARGET = 0x6,
	IMG_CATEGORY_TEMP = 0x7,
};


enum MaterialShaderArgumentType : unsigned __int16
{
	MTL_ARG_MATERIAL_VERTEX_CONST = 0x0,
	MTL_ARG_LITERAL_VERTEX_CONST = 0x1,
	MTL_ARG_MATERIAL_PIXEL_SAMPLER = 0x2,
	MTL_ARG_CODE_PRIM_BEGIN = 0x3,
	MTL_ARG_CODE_VERTEX_CONST = 0x3,
	MTL_ARG_CODE_PIXEL_SAMPLER = 0x4,
	MTL_ARG_CODE_PIXEL_CONST = 0x5,
	MTL_ARG_CODE_PRIM_END = 0x6,
	MTL_ARG_MATERIAL_PIXEL_CONST = 0x6,
	MTL_ARG_LITERAL_PIXEL_CONST = 0x7,
	MTL_ARG_COUNT = 0x8,
};

enum MaterialUpdateFrequency
{
	MTL_UPDATE_PER_PRIM = 0x0,
	MTL_UPDATE_PER_OBJECT = 0x1,
	MTL_UPDATE_RARELY = 0x2,
	MTL_UPDATE_CUSTOM = 0x3,
};

enum GfxImageFileFormat
{
	IMG_FORMAT_INVALID = 0x0,
	IMG_FORMAT_BITMAP_RGBA = 0x1,
	IMG_FORMAT_BITMAP_RGB = 0x2,
	IMG_FORMAT_BITMAP_LUMINANCE_ALPHA = 0x3,
	IMG_FORMAT_BITMAP_LUMINANCE = 0x4,
	IMG_FORMAT_BITMAP_ALPHA = 0x5,
	IMG_FORMAT_WAVELET_RGBA = 0x6,
	IMG_FORMAT_WAVELET_RGB = 0x7,
	IMG_FORMAT_WAVELET_LUMINANCE_ALPHA = 0x8,
	IMG_FORMAT_WAVELET_LUMINANCE = 0x9,
	IMG_FORMAT_WAVELET_ALPHA = 0xA,
	IMG_FORMAT_DXT1 = 0xB,
	IMG_FORMAT_DXT3 = 0xC,
	IMG_FORMAT_DXT5 = 0xD,
	IMG_FORMAT_DXN = 0xE,
	IMG_FORMAT_COUNT = 0xF
};


union IW3SoundAliasFlags
{
	struct
	{
		unsigned int looping : 1;
		unsigned int isMaster : 1;
		unsigned int isSlave : 1;
		unsigned int fullDryLevel : 1;
		unsigned int noWetLevel : 1;
		unsigned int randomLooping : 1;
		unsigned int type : 2;
		unsigned int channel : 6;
	};
	unsigned int intValue;
};

enum snd_alias_type_t
{
	SAT_UNKNOWN = 0x0,
	SAT_LOADED = 0x1,
	SAT_STREAMED = 0x2,
	SAT_COUNT = 0x3,
};

struct cmd_function_s
{
	cmd_function_s* next;
	const char* name;
	const char* autoCompleteDir;
	const char* autoCompleteExt;
	void(__cdecl* function)();
};

union DvarLimits
{
	struct
	{
		int stringCount;
		const char** strings;
	} enumeration;

	struct
	{
		int min;
		int max;
	} integer;

	struct
	{
		float min;
		float max;
	} value;

	struct
	{
		float min;
		float max;
	} vector;
};

union DvarValue
{
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	float value;
	float vector[4];
	const char* string;
	char color[4];
};

enum DvarType
{
	DVAR_TYPE_BOOL = 0x0,
	DVAR_TYPE_FLOAT = 0x1,
	DVAR_TYPE_FLOAT_2 = 0x2,
	DVAR_TYPE_FLOAT_3 = 0x3,
	DVAR_TYPE_FLOAT_4 = 0x4,
	DVAR_TYPE_INT = 0x5,
	DVAR_TYPE_ENUM = 0x6,
	DVAR_TYPE_STRING = 0x7,
	DVAR_TYPE_COLOR = 0x8,
	DVAR_TYPE_COUNT = 0x9,
};

struct dvar_s
{
	const char* name;
	const char* description;
	unsigned __int16 flags;
	char type;
	bool modified;
	DvarValue current;
	DvarValue latched;
	DvarValue reset;
	DvarLimits domain;
	bool(__cdecl* domainFunc)(dvar_s*, DvarValue);
	dvar_s* hashNext;
};

static const MaterialUpdateFrequency codeConstUpdateFrequency[]
{
	MTL_UPDATE_RARELY, // LIGHT_POSITION 
	MTL_UPDATE_RARELY, // LIGHT_DIFFUSE 
	MTL_UPDATE_RARELY, // LIGHT_SPECULAR 
	MTL_UPDATE_RARELY, // LIGHT_SPOTDIR 
	MTL_UPDATE_RARELY, // LIGHT_SPOTFACTORS 
	MTL_UPDATE_RARELY, // LIGHT_FALLOFF_PLACEMENT 
	MTL_UPDATE_RARELY, // PARTICLE_CLOUD_COLOR 
	MTL_UPDATE_RARELY, // GAMETIME 
	MTL_UPDATE_RARELY, // PIXEL_COST_FRACS 
	MTL_UPDATE_RARELY, // PIXEL_COST_DECODE 
	MTL_UPDATE_RARELY, // FILTER_TAP_0 
	MTL_UPDATE_RARELY, // FILTER_TAP_1 
	MTL_UPDATE_RARELY, // FILTER_TAP_2 
	MTL_UPDATE_RARELY, // FILTER_TAP_3 
	MTL_UPDATE_RARELY, // FILTER_TAP_4 
	MTL_UPDATE_RARELY, // FILTER_TAP_5 
	MTL_UPDATE_RARELY, // FILTER_TAP_6 
	MTL_UPDATE_RARELY, // FILTER_TAP_7 
	MTL_UPDATE_RARELY, // COLOR_MATRIX_R 
	MTL_UPDATE_RARELY, // COLOR_MATRIX_G 
	MTL_UPDATE_RARELY, // COLOR_MATRIX_B 
	MTL_UPDATE_RARELY, // SHADOWMAP_POLYGON_OFFSET 
	MTL_UPDATE_RARELY, // RENDER_TARGET_SIZE 
	MTL_UPDATE_RARELY, // DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR 
	MTL_UPDATE_RARELY, // DOF_EQUATION_SCENE 
	MTL_UPDATE_RARELY, // DOF_LERP_SCALE 
	MTL_UPDATE_RARELY, // DOF_LERP_BIAS 
	MTL_UPDATE_RARELY, // DOF_ROW_DELTA 
	MTL_UPDATE_RARELY, // MOTION_MATRIX_X 
	MTL_UPDATE_RARELY, // MOTION_MATRIX_Y 
	MTL_UPDATE_RARELY, // MOTION_MATRIX_W 
	MTL_UPDATE_RARELY, // SHADOWMAP_SWITCH_PARTITION 
	MTL_UPDATE_RARELY, // SHADOWMAP_SCALE 
	MTL_UPDATE_RARELY, // ZNEAR 
	MTL_UPDATE_RARELY, // LIGHTING_LOOKUP_SCALE 
	MTL_UPDATE_RARELY, // DEBUG_BUMPMAP 
	MTL_UPDATE_RARELY, // MATERIAL_COLOR 
	MTL_UPDATE_RARELY, // FOG 
	MTL_UPDATE_RARELY, // FOG_COLOR_LINEAR 
	MTL_UPDATE_RARELY, // FOG_COLOR_GAMMA 
	MTL_UPDATE_RARELY, // FOG_SUN_CONSTS 
	MTL_UPDATE_RARELY, // FOG_SUN_COLOR_LINEAR 
	MTL_UPDATE_RARELY, // FOG_SUN_COLOR_GAMMA 
	MTL_UPDATE_RARELY, // FOG_SUN_DIR 
	MTL_UPDATE_RARELY, // GLOW_SETUP 
	MTL_UPDATE_RARELY, // GLOW_APPLY 
	MTL_UPDATE_RARELY, // COLOR_BIAS 
	MTL_UPDATE_RARELY, // COLOR_TINT_BASE 
	MTL_UPDATE_RARELY, // COLOR_TINT_DELTA 
	MTL_UPDATE_RARELY, // COLOR_TINT_QUADRATIC_DELTA 
	MTL_UPDATE_RARELY, // OUTDOOR_FEATHER_PARMS 
	MTL_UPDATE_RARELY, // ENVMAP_PARMS 
	MTL_UPDATE_RARELY, // SUN_SHADOWMAP_PIXEL_ADJUST 
	MTL_UPDATE_RARELY, // SPOT_SHADOWMAP_PIXEL_ADJUST 
	MTL_UPDATE_RARELY, // COMPOSITE_FX_DISTORTION 
	MTL_UPDATE_RARELY, // POSTFX_FADE_EFFECT 
	MTL_UPDATE_RARELY, // VIEWPORT_DIMENSIONS 
	MTL_UPDATE_RARELY, // FRAMEBUFFER_READ 
	MTL_UPDATE_PER_PRIM, // BASE_LIGHTING_COORDS 
	MTL_UPDATE_PER_PRIM, // LIGHT_PROBE_AMBIENT 
	MTL_UPDATE_RARELY, // NEARPLANE_ORG 
	MTL_UPDATE_RARELY, // NEARPLANE_DX 
	MTL_UPDATE_RARELY, // NEARPLANE_DY 
	MTL_UPDATE_RARELY, // CLIP_SPACE_LOOKUP_SCALE 
	MTL_UPDATE_RARELY, // CLIP_SPACE_LOOKUP_OFFSET 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX0 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX1 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_MATRIX2 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR0 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR1 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_CLOUD_SPARK_COLOR2 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_FOUNTAIN_PARM0 
	MTL_UPDATE_PER_OBJECT, // PARTICLE_FOUNTAIN_PARM1 
	MTL_UPDATE_PER_OBJECT, // DEPTH_FROM_CLIP 
	MTL_UPDATE_PER_OBJECT, // CODE_MESH_ARG_0 
	MTL_UPDATE_PER_OBJECT, // CODE_MESH_ARG_1 
	MTL_UPDATE_PER_OBJECT, // VIEW_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_VIEW_MATRIX 
	MTL_UPDATE_PER_OBJECT, // TRANSPOSE_VIEW_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_VIEW_MATRIX 
	MTL_UPDATE_PER_OBJECT, // PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // TRANSPOSE_PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // VIEW_PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_VIEW_PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // TRANSPOSE_VIEW_PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX 
	MTL_UPDATE_PER_OBJECT, // SHADOW_LOOKUP_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_SHADOW_LOOKUP_MATRIX 
	MTL_UPDATE_PER_OBJECT, // TRANSPOSE_SHADOW_LOOKUP_MATRIX 
	MTL_UPDATE_PER_OBJECT, // INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX 
	MTL_UPDATE_PER_PRIM, // WORLD_OUTDOOR_LOOKUP_MATRIX 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX 
	MTL_UPDATE_PER_PRIM, // WORLD_MATRIX0 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX0 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX0 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX0 
	MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX0 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX0 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX0 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 
	MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX0 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 
	MTL_UPDATE_PER_PRIM, // WORLD_MATRIX1 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX1 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX1 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX1 
	MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX1 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX1 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX1 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1 
	MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX1 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX1 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 
	MTL_UPDATE_PER_PRIM, // WORLD_MATRIX2 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_MATRIX2 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_MATRIX2 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_MATRIX2 
	MTL_UPDATE_PER_PRIM, // WORLD_VIEW_MATRIX2 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_MATRIX2 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_MATRIX2 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2 
	MTL_UPDATE_PER_PRIM, // WORLD_VIEW_PROJECTION_MATRIX2 
	MTL_UPDATE_PER_PRIM, // INVERSE_WORLD_VIEW_PROJECTION_MATRIX2 
	MTL_UPDATE_PER_PRIM, // TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 
	MTL_UPDATE_PER_PRIM, // INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 
};


static const MaterialUpdateFrequency codeSamplerUpdateFrequency[]
{
	MTL_UPDATE_RARELY, // BLACK 
	MTL_UPDATE_RARELY, // WHITE 
	MTL_UPDATE_RARELY, // IDENTITY_NORMAL_MAP 
	MTL_UPDATE_RARELY, // MODEL_LIGHTING 
	MTL_UPDATE_CUSTOM, // LIGHTMAP_PRIMARY 
	MTL_UPDATE_CUSTOM, // LIGHTMAP_SECONDARY 
	MTL_UPDATE_RARELY, // SHADOWMAP_SUN 
	MTL_UPDATE_RARELY, // SHADOWMAP_SPOT 
	MTL_UPDATE_PER_OBJECT, // FEEDBACK 
	MTL_UPDATE_RARELY, // RESOLVED_POST_SUN 
	MTL_UPDATE_RARELY, // RESOLVED_SCENE 
	MTL_UPDATE_RARELY, // POST_EFFECT_0 
	MTL_UPDATE_RARELY, // POST_EFFECT_1 
	MTL_UPDATE_PER_OBJECT, // LIGHT_ATTENUATION 
	MTL_UPDATE_RARELY, // OUTDOOR 
	MTL_UPDATE_RARELY, // FLOATZ 
	MTL_UPDATE_RARELY, // PROCESSED_FLOATZ 
	MTL_UPDATE_RARELY, // RAW_FLOATZ 
	MTL_UPDATE_RARELY, // HALF_PARTICLES 
	MTL_UPDATE_RARELY, // HALF_PARTICLES_Z 
	MTL_UPDATE_PER_OBJECT, // CASE_TEXTURE 
	MTL_UPDATE_PER_OBJECT, // CINEMATIC_Y 
	MTL_UPDATE_PER_OBJECT, // CINEMATIC_CR 
	MTL_UPDATE_PER_OBJECT, // CINEMATIC_CB 
	MTL_UPDATE_PER_OBJECT, // CINEMATIC_A 
	MTL_UPDATE_CUSTOM, // REFLECTION_PROBE 
	MTL_UPDATE_RARELY, // ALTERNATE_SCENE 
};

struct XZoneInfo
{
	const char* name;
	int allocFlags;
	int freeFlags;
};

#pragma region SplitTypes

#ifdef __cplusplus
namespace IW3
{
#endif
	enum XAssetType
	{
		ASSET_TYPE_XMODELPIECES = 0x0,
		ASSET_TYPE_PHYSPRESET = 0x1,
		ASSET_TYPE_XANIMPARTS = 0x2,
		ASSET_TYPE_XMODEL = 0x3,
		ASSET_TYPE_MATERIAL = 0x4,
		ASSET_TYPE_TECHNIQUE_SET = 0x5,
		ASSET_TYPE_IMAGE = 0x6,
		ASSET_TYPE_SOUND = 0x7,
		ASSET_TYPE_SOUND_CURVE = 0x8,
		ASSET_TYPE_LOADED_SOUND = 0x9,
		ASSET_TYPE_CLIPMAP = 0xA,
		ASSET_TYPE_CLIPMAP_PVS = 0xB,
		ASSET_TYPE_COMWORLD = 0xC,
		ASSET_TYPE_GAMEWORLD_SP = 0xD,
		ASSET_TYPE_GAMEWORLD_MP = 0xE,
		ASSET_TYPE_MAP_ENTS = 0xF,
		ASSET_TYPE_GFXWORLD = 0x10,
		ASSET_TYPE_LIGHT_DEF = 0x11,
		ASSET_TYPE_UI_MAP = 0x12,
		ASSET_TYPE_FONT = 0x13,
		ASSET_TYPE_MENULIST = 0x14,
		ASSET_TYPE_MENU = 0x15,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x16,
		ASSET_TYPE_WEAPON = 0x17,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x18,
		ASSET_TYPE_FX = 0x19,
		ASSET_TYPE_IMPACT_FX = 0x1A,
		ASSET_TYPE_AITYPE = 0x1B,
		ASSET_TYPE_MPTYPE = 0x1C,
		ASSET_TYPE_CHARACTER = 0x1D,
		ASSET_TYPE_XMODELALIAS = 0x1E,
		ASSET_TYPE_RAWFILE = 0x1F,
		ASSET_TYPE_STRINGTABLE = 0x20,
		ASSET_TYPE_COUNT = 0x21,
		ASSET_TYPE_STRING = 0x21,
		ASSET_TYPE_ASSETLIST = 0x22,
	};
	
	enum FreeFlags
	{
	  DB_ZONE_CODE_LOC = 0x0,
	  DB_ZONE_COMMON_LOC = 0x1,
	  DB_ZONE_CODE = 0x2,
	  DB_ZONE_COMMON = 0x4,
	  DB_ZONE_GAME = 0x8,
	  DB_ZONE_MOD = 0x10,
	  DB_ZONE_LOAD = 0x20,
	  DB_ZONE_DEV = 0x40,
	};

	enum StaticModelFlag : char
	{
		STATIC_MODEL_FLAG_NO_SHADOW = 0x1,
	};

	enum SndChannel
	{
		SND_CHANNEL_PHYSICS,
		SND_CHANNEL_AUTO,
		SND_CHANNEL_AUTO2,
		SND_CHANNEL_AUTODOG,
		SND_CHANNEL_BULLETIMPACT,
		SND_CHANNEL_BULLETWHIZBY,
		SND_CHANNEL_ELEMENT,
		SND_CHANNEL_AUTO2D,
		SND_CHANNEL_VEHICLE,
		SND_CHANNEL_VEHICLELIMITED,
		SND_CHANNEL_MENU,
		SND_CHANNEL_BODY,
		SND_CHANNEL_BODY2D,
		SND_CHANNEL_RELOAD,
		SND_CHANNEL_RELOAD2D,
		SND_CHANNEL_ITEM,
		SND_CHANNEL_EFFECTS1,
		SND_CHANNEL_EFFECTS2,
		SND_CHANNEL_WEAPON,
		SND_CHANNEL_WEAPON2D,
		SND_CHANNEL_NONSHOCK,
		SND_CHANNEL_VOICE,
		SND_CHANNEL_LOCAL,
		SND_CHANNEL_LOCAL2,
		SND_CHANNEL_AMBIENT,
		SND_CHANNEL_HURT,
		SND_CHANNEL_PLAYER1,
		SND_CHANNEL_PLAYER2,
		SND_CHANNEL_MUSIC,
		SND_CHANNEL_MUSICNOPAUSE,
		SND_CHANNEL_MISSION,
		SND_CHANNEL_ANNOUNCER,
		SND_CHANNEL_SHELLSHOCK,

		SND_CHANNEL_COUNT
	};

	enum MapType
	{
		MAPTYPE_NONE = 0x0,
		MAPTYPE_INVALID1 = 0x1,
		MAPTYPE_INVALID2 = 0x2,
		MAPTYPE_2D = 0x3,
		MAPTYPE_3D = 0x4,
		MAPTYPE_CUBE = 0x5,
		MAPTYPE_COUNT = 0x6,
	};

	struct Picmip
	{
		char platform[2];
	};

	struct CardMemory
	{
		int platform[2];
	};

	struct GfxImageLoadDef
	{
		char levelCount;
		char flags;
		__int16 dimensions[3];
		int format;
		int resourceSize;
		unsigned char data[1];
	};

	union GfxTexture
	{
		/*IDirect3DBaseTexture9 *basemap;
		IDirect3DTexture9 *map;
		IDirect3DVolumeTexture9 *volmap;
		IDirect3DCubeTexture9 *cubemap;*/
		GfxImageLoadDef* loadDef;
		void* data;
	};

	struct GfxImage
	{
		MapType mapType;
		GfxTexture texture;
		Picmip picmip;
		bool noPicmip;
		TextureSemantic semantic;
		char track;
		CardMemory cardMemory;
		unsigned __int16 width;
		unsigned __int16 height;
		unsigned __int16 depth;
		GfxImageCategory category;
		bool delayLoadPixels;
		const char* name;
	};

	union SoundAliasFlags
	{
		struct
		{
			unsigned int looping : 1;
			unsigned int isMaster : 1;
			unsigned int isSlave : 1;
			unsigned int fullDryLevel : 1;
			unsigned int noWetLevel : 1;
			unsigned int unknown : 1;
			unsigned int type : 2;
			unsigned int channel : 6;
		};
		unsigned int intValue;
	};

	enum file_image_flags_t
	{
		IMG_FLAG_NOPICMIP = 0x1,
		IMG_FLAG_NOMIPMAPS = 0x2,
		IMG_FLAG_CUBEMAP = 0x4,
		IMG_FLAG_VOLMAP = 0x8,
		IMG_FLAG_STREAMING = 0x10,
		IMG_FLAG_LEGACY_NORMALS = 0x20,
		IMG_FLAG_CLAMP_U = 0x40,
		IMG_FLAG_CLAMP_V = 0x80,
		IMG_FLAG_DYNAMIC = 0x10000,
		IMG_FLAG_RENDER_TARGET = 0x20000,
		IMG_FLAG_SYSTEMMEM = 0x40000,
	};

	struct GfxImageFileHeader
	{
		char tag[3];
		char version;
		char format;
		char flags;
		short dimensions[3];
		int fileSizeForPicmip[4];
	};

	enum GfxRenderer : unsigned __int16
	{
		GFX_RENDERER_SHADER_2 = 0x0,
		GFX_RENDERER_SHADER_3 = 0x1,
		GFX_RENDERER_COUNT = 0x2,
		GFX_RENDERER_UNSPECIFIED = 0x2,
	};

	struct GfxVertexShaderLoadDef
	{
		unsigned int* program;
		unsigned __int16 programSize;
		GfxRenderer loadForRenderer;
	};

	struct MaterialVertexShaderProgram
	{
		void* vs;
		GfxVertexShaderLoadDef loadDef;
	};

	struct MaterialVertexShader
	{
		const char* name;
		MaterialVertexShaderProgram prog;
	};

	struct GfxPixelShaderLoadDef
	{
		unsigned int* program;
		unsigned __int16 programSize;
		GfxRenderer loadForRenderer;
	};

	struct MaterialPixelShaderProgram
	{
		void* ps;
		GfxPixelShaderLoadDef loadDef;
	};

	struct MaterialPixelShader
	{
		const char* name;
		MaterialPixelShaderProgram prog;
	};

	struct MaterialConstantDef
	{
		int nameHash;
		char name[12];
		vec4_t literal;
	};


	enum GfxSurfaceStatebitOp0 : unsigned int
	{
		GFXS0_SRCBLEND_RGB_SHIFT = 0x0,
		GFXS0_SRCBLEND_RGB_MASK = 0xF,
		GFXS0_DSTBLEND_RGB_SHIFT = 0x4,
		GFXS0_DSTBLEND_RGB_MASK = 0xF0,
		GFXS0_BLENDOP_RGB_SHIFT = 0x8,
		GFXS0_BLENDOP_RGB_MASK = 0x700,
		GFXS0_BLEND_RGB_MASK = 0x7FF,
		GFXS0_ATEST_DISABLE = 0x800,
		GFXS0_ATEST_GT_0 = 0x1000,
		GFXS0_ATEST_LT_128 = 0x2000,
		GFXS0_ATEST_GE_128 = 0x3000,
		GFXS0_ATEST_MASK = 0x3000,
		GFXS0_CULL_SHIFT = 0xE,
		GFXS0_CULL_NONE = 0x4000,
		GFXS0_CULL_BACK = 0x8000,
		GFXS0_CULL_FRONT = 0xC000,
		GFXS0_CULL_MASK = 0xC000,
		GFXS0_SRCBLEND_ALPHA_SHIFT = 0x10,
		GFXS0_SRCBLEND_ALPHA_MASK = 0xF0000,
		GFXS0_DSTBLEND_ALPHA_SHIFT = 0x14,
		GFXS0_DSTBLEND_ALPHA_MASK = 0xF00000,
		GFXS0_BLENDOP_ALPHA_SHIFT = 0x18,
		GFXS0_BLENDOP_ALPHA_MASK = 0x7000000,
		GFXS0_BLEND_ALPHA_MASK = 0x7FF0000,
		GFXS0_COLORWRITE_RGB = 0x8000000,
		GFXS0_COLORWRITE_ALPHA = 0x10000000,
		GFXS0_COLORWRITE_MASK = 0x18000000,
		GFXS0_POLYMODE_LINE = 0x80000000
	};

	enum GfxSurfaceStatebitOp1 : unsigned int
	{
		GFXS1_DEPTHWRITE = 0x1,
		GFXS1_DEPTHTEST_DISABLE = 0x2,
		GFXS1_DEPTHTEST_SHIFT = 0x2,
		GFXS1_DEPTHTEST_ALWAYS = 0x0,
		GFXS1_DEPTHTEST_LESS = 0x4,
		GFXS1_DEPTHTEST_EQUAL = 0x8,
		GFXS1_DEPTHTEST_LESSEQUAL = 0xC,
		GFXS1_DEPTHTEST_MASK = 0xC,
		GFXS1_POLYGON_OFFSET_SHIFT = 0x4,
		GFXS1_POLYGON_OFFSET_0 = 0x0,
		GFXS1_POLYGON_OFFSET_1 = 0x10,
		GFXS1_POLYGON_OFFSET_2 = 0x20,
		GFXS1_POLYGON_OFFSET_SHADOWMAP = 0x30,
		GFXS1_POLYGON_OFFSET_MASK = 0x30,
		GFXS1_STENCIL_FRONT_ENABLE = 0x40,
		GFXS1_STENCIL_BACK_ENABLE = 0x80,
		GFXS1_STENCIL_MASK = 0xC0,
		GFXS1_STENCIL_FRONT_PASS_SHIFT = 0x8,
		GFXS1_STENCIL_FRONT_FAIL_SHIFT = 0xB,
		GFXS1_STENCIL_FRONT_ZFAIL_SHIFT = 0xE,
		GFXS1_STENCIL_FRONT_FUNC_SHIFT = 0x11,
		GFXS1_STENCIL_FRONT_MASK = 0xFFF00,
		GFXS1_STENCIL_BACK_PASS_SHIFT = 0x14,
		GFXS1_STENCIL_BACK_FAIL_SHIFT = 0x17,
		GFXS1_STENCIL_BACK_ZFAIL_SHIFT = 0x1A,
		GFXS1_STENCIL_BACK_FUNC_SHIFT = 0x1D,
		GFXS1_STENCIL_BACK_MASK = 0xFFF00000,
		GFXS1_STENCILFUNC_FRONTBACK_MASK = 0xE00E0000,
		GFXS1_STENCILOP_FRONTBACK_MASK = 0x1FF1FF00,
	};

	enum GfxStencilOp
	{
		GFXS_STENCILOP_KEEP = 0x0,
		GFXS_STENCILOP_ZERO = 0x1,
		GFXS_STENCILOP_REPLACE = 0x2,
		GFXS_STENCILOP_INCRSAT = 0x3,
		GFXS_STENCILOP_DECRSAT = 0x4,
		GFXS_STENCILOP_INVERT = 0x5,
		GFXS_STENCILOP_INCR = 0x6,
		GFXS_STENCILOP_DECR = 0x7,

		GFXS_STENCILOP_COUNT,
		GFXS_STENCILOP_MASK = 0x7
	};

	struct GfxStatebitsFlags {
		GfxSurfaceStatebitOp0 loadbit0;
		GfxSurfaceStatebitOp1 loadbit1;
	};

	union GfxStateBits
	{
		GfxStatebitsFlags flags;
		int loadbits[2];
	};

	struct WaterWritable
	{
		float floatTime;
	};

	struct complex_s
	{
		float real;
		float imag;
	};

	struct water_t
	{
		WaterWritable writable;
		complex_s* H0;
		float* wTerm;
		int M;
		int N;
		float Lx;
		float Lz;
		float gravity;
		float windvel;
		float winddir[2];
		float amplitude;
		float codeConstant[4];
		GfxImage* image;
	};

	/* MaterialTextureDef->semantic */
#define TS_2D           0x0
#define TS_FUNCTION     0x1
#define TS_COLOR_MAP    0x2
#define TS_UNUSED_1     0x3
#define TS_UNUSED_2     0x4
#define TS_NORMAL_MAP   0x5
#define TS_UNUSED_3     0x6
#define TS_UNUSED_4     0x7
#define TS_SPECULAR_MAP 0x8
#define TS_UNUSED_5     0x9
#define TS_UNUSED_6     0xA
#define TS_WATER_MAP    0xB

	union MaterialTextureDefInfo
	{
		GfxImage* image;	// MaterialTextureDef->semantic != TS_WATER_MAP
		water_t* water;		// MaterialTextureDef->semantic == TS_WATER_MAP
	};

	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		char samplerState;
		TextureSemantic semantic;
		MaterialTextureDefInfo u;
	};

	struct GfxDrawSurfFields
	{
		unsigned __int64 objectId : 16;
		unsigned __int64 reflectionProbeIndex : 8;
		unsigned __int64 customIndex : 5;
		unsigned __int64 materialSortedIndex : 11;
		unsigned __int64 prepass : 2;
		unsigned __int64 primaryLightIndex : 8;
		unsigned __int64 surfType : 4;
		unsigned __int64 primarySortKey : 6;
		unsigned __int64 unused : 4;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned long long packed;
	};

	struct MaterialArgumentCodeConst
	{
		unsigned __int16 index;
		char firstRow;
		char rowCount;
	};

	union MaterialArgumentDef
	{
		float* literalConst;
		MaterialArgumentCodeConst codeConst;
		unsigned int codeSampler;
		unsigned int nameHash;
	};

	struct MaterialShaderArgument
	{
		MaterialShaderArgumentType type;
		unsigned __int16 dest;
		MaterialArgumentDef u;
	};

	enum ShaderCodeConstants
	{
		CONST_SRC_CODE_MAYBE_DIRTY_PS_BEGIN = 0x0,
		CONST_SRC_CODE_LIGHT_POSITION = 0x0,
		CONST_SRC_CODE_LIGHT_DIFFUSE = 0x1,
		CONST_SRC_CODE_LIGHT_SPECULAR = 0x2,
		CONST_SRC_CODE_LIGHT_SPOTDIR = 0x3,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS = 0x4,
		CONST_SRC_CODE_NEARPLANE_ORG = 0x5,
		CONST_SRC_CODE_NEARPLANE_DX = 0x6,
		CONST_SRC_CODE_NEARPLANE_DY = 0x7,
		CONST_SRC_CODE_SHADOW_PARMS = 0x8,
		CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET = 0x9,
		CONST_SRC_CODE_RENDER_TARGET_SIZE = 0xA,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT = 0xB,
		CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR = 0xC,
		CONST_SRC_CODE_DOF_EQUATION_SCENE = 0xD,
		CONST_SRC_CODE_DOF_LERP_SCALE = 0xE,
		CONST_SRC_CODE_DOF_LERP_BIAS = 0xF,
		CONST_SRC_CODE_DOF_ROW_DELTA = 0x10,
		CONST_SRC_CODE_PARTICLE_CLOUD_COLOR = 0x11,
		CONST_SRC_CODE_GAMETIME = 0x12,
		CONST_SRC_CODE_MAYBE_DIRTY_PS_END = 0x13,
		CONST_SRC_CODE_ALWAYS_DIRTY_PS_BEGIN = 0x13,
		CONST_SRC_CODE_PIXEL_COST_FRACS = 0x13,
		CONST_SRC_CODE_PIXEL_COST_DECODE = 0x14,
		CONST_SRC_CODE_FILTER_TAP_0 = 0x15,
		CONST_SRC_CODE_FILTER_TAP_1 = 0x16,
		CONST_SRC_CODE_FILTER_TAP_2 = 0x17,
		CONST_SRC_CODE_FILTER_TAP_3 = 0x18,
		CONST_SRC_CODE_FILTER_TAP_4 = 0x19,
		CONST_SRC_CODE_FILTER_TAP_5 = 0x1A,
		CONST_SRC_CODE_FILTER_TAP_6 = 0x1B,
		CONST_SRC_CODE_FILTER_TAP_7 = 0x1C,
		CONST_SRC_CODE_COLOR_MATRIX_R = 0x1D,
		CONST_SRC_CODE_COLOR_MATRIX_G = 0x1E,
		CONST_SRC_CODE_COLOR_MATRIX_B = 0x1F,
		CONST_SRC_CODE_ALWAYS_DIRTY_PS_END = 0x20,
		CONST_SRC_CODE_NEVER_DIRTY_PS_BEGIN = 0x20,
		CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION = 0x20,
		CONST_SRC_CODE_SHADOWMAP_SCALE = 0x21,
		CONST_SRC_CODE_ZNEAR = 0x22,
		CONST_SRC_CODE_SUN_POSITION = 0x23,
		CONST_SRC_CODE_SUN_DIFFUSE = 0x24,
		CONST_SRC_CODE_SUN_SPECULAR = 0x25,
		CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE = 0x26,
		CONST_SRC_CODE_DEBUG_BUMPMAP = 0x27,
		CONST_SRC_CODE_MATERIAL_COLOR = 0x28,
		CONST_SRC_CODE_FOG = 0x29,
		CONST_SRC_CODE_FOG_COLOR = 0x2A,
		CONST_SRC_CODE_GLOW_SETUP = 0x2B,
		CONST_SRC_CODE_GLOW_APPLY = 0x2C,
		CONST_SRC_CODE_COLOR_BIAS = 0x2D,
		CONST_SRC_CODE_COLOR_TINT_BASE = 0x2E,
		CONST_SRC_CODE_COLOR_TINT_DELTA = 0x2F,
		CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS = 0x30,
		CONST_SRC_CODE_ENVMAP_PARMS = 0x31,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST = 0x32,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE = 0x33,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET = 0x34,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX = 0x35,
		CONST_SRC_CODE_DEPTH_FROM_CLIP = 0x36,
		CONST_SRC_CODE_CODE_MESH_ARG_0 = 0x37,
		CONST_SRC_CODE_CODE_MESH_ARG_1 = 0x38,
		CONST_SRC_CODE_CODE_MESH_ARG_LAST = 0x38,
		CONST_SRC_CODE_BASE_LIGHTING_COORDS = 0x39,
		CONST_SRC_CODE_NEVER_DIRTY_PS_END = 0x3A,
		CONST_SRC_CODE_COUNT_FLOAT4 = 0x3A,
		CONST_SRC_FIRST_CODE_MATRIX = 0x3A,
		CONST_SRC_CODE_WORLD_MATRIX = 0x3A,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX = 0x3B,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX = 0x3C,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX = 0x3D,
		CONST_SRC_CODE_VIEW_MATRIX = 0x3E,
		CONST_SRC_CODE_INVERSE_VIEW_MATRIX = 0x3F,
		CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX = 0x40,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX = 0x41,
		CONST_SRC_CODE_PROJECTION_MATRIX = 0x42,
		CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX = 0x43,
		CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX = 0x44,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX = 0x45,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX = 0x46,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX = 0x47,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX = 0x48,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX = 0x49,
		CONST_SRC_CODE_VIEW_PROJECTION_MATRIX = 0x4A,
		CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX = 0x4B,
		CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x4C,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x4D,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX = 0x4E,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX = 0x4F,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX = 0x50,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX = 0x51,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX = 0x52,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX = 0x53,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x54,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x55,
		CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x56,
		CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x57,
		CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x58,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x59,
		CONST_SRC_TOTAL_COUNT = 0x5A,
		CONST_SRC_NONE = 0x5B,
	};

	enum GfxRenderTargetId
	{
		R_RENDERTARGET_SAVED_SCREEN = 0x0,
		R_RENDERTARGET_FRAME_BUFFER = 0x1,
		R_RENDERTARGET_SCENE = 0x2,
		R_RENDERTARGET_RESOLVED_POST_SUN = 0x3,
		R_RENDERTARGET_RESOLVED_SCENE = 0x4,
		R_RENDERTARGET_FLOAT_Z = 0x5,
		R_RENDERTARGET_DYNAMICSHADOWS = 0x6,
		R_RENDERTARGET_PINGPONG_0 = 0x7,
		R_RENDERTARGET_PINGPONG_1 = 0x8,
		R_RENDERTARGET_SHADOWCOOKIE = 0x9,
		R_RENDERTARGET_SHADOWCOOKIE_BLUR = 0xA,
		R_RENDERTARGET_POST_EFFECT_0 = 0xB,
		R_RENDERTARGET_POST_EFFECT_1 = 0xC,
		R_RENDERTARGET_SHADOWMAP_SUN = 0xD,
		R_RENDERTARGET_SHADOWMAP_SPOT = 0xE,
		R_RENDERTARGET_COUNT = 0xF,
		R_RENDERTARGET_NONE = 0x10,
	};


	enum MaterialStreamRoutingSource : unsigned char
	{
		STREAM_SRC_POSITION = 0x0,
		STREAM_SRC_COLOR = 0x1,
		STREAM_SRC_TEXCOORD_0 = 0x2,
		STREAM_SRC_NORMAL = 0x3,
		STREAM_SRC_TANGENT = 0x4,
		STREAM_SRC_OPTIONAL_BEGIN = 0x5,
		STREAM_SRC_PRE_OPTIONAL_BEGIN = 0x4,
		STREAM_SRC_TEXCOORD_1 = 0x5,
		STREAM_SRC_TEXCOORD_2 = 0x6,
		STREAM_SRC_NORMAL_TRANSFORM_0 = 0x7,
		STREAM_SRC_NORMAL_TRANSFORM_1 = 0x8,
		STREAM_SRC_COUNT = 0x9,
	};

	enum MaterialStreamRoutingDestination : unsigned char
	{
		STREAM_DST_POSITION = 0x0,
		STREAM_DST_NORMAL = 0x1,
		STREAM_DST_COLOR_0 = 0x2,
		STREAM_DST_COLOR_1 = 0x3,
		STREAM_DST_TEXCOORD_0 = 0x4,
		STREAM_DST_TEXCOORD_1 = 0x5,
		STREAM_DST_TEXCOORD_2 = 0x6,
		STREAM_DST_TEXCOORD_3 = 0x7,
		STREAM_DST_TEXCOORD_4 = 0x8,
		STREAM_DST_TEXCOORD_5 = 0x9,
		STREAM_DST_TEXCOORD_6 = 0xA,
		STREAM_DST_TEXCOORD_7 = 0xB,
		STREAM_DST_COUNT = 0xC,
	};

	struct MaterialStreamRouting
	{
		MaterialStreamRoutingSource source;
		MaterialStreamRoutingDestination dest;
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[16];
		void* decl[16];
	};

	struct MaterialVertexDeclaration
	{
		char streamCount;
		bool hasOptionalSource;
		bool isLoaded;
		MaterialVertexStreamRouting routing;
	};

#pragma pack(push, 4)
	struct MaterialPass
	{
		MaterialVertexDeclaration* vertexDecl;
		MaterialVertexShader* vertexShader;
		MaterialPixelShader* pixelShader;
		char perPrimArgCount;
		char perObjArgCount;
		char stableArgCount;
		char customSamplerFlags;
		MaterialShaderArgument* args;
	};
#pragma pack(pop)

	struct MaterialTechnique
	{
		const char* name;
		unsigned __int16 flags;
		unsigned __int16 passCount;
		MaterialPass passArray[1];	// count = passCount
	};

	/* MaterialTechniqueSet->worldVertFormat */
	enum MaterialWorldVertexFormat : char
	{
		MTL_WORLDVERT_TEX_1_NRM_1 = 0x0,
		MTL_WORLDVERT_TEX_2_NRM_1 = 0x1,
		MTL_WORLDVERT_TEX_2_NRM_2 = 0x2,
		MTL_WORLDVERT_TEX_3_NRM_1 = 0x3,
		MTL_WORLDVERT_TEX_3_NRM_2 = 0x4,
		MTL_WORLDVERT_TEX_3_NRM_3 = 0x5,
		MTL_WORLDVERT_TEX_4_NRM_1 = 0x6,
		MTL_WORLDVERT_TEX_4_NRM_2 = 0x7,
		MTL_WORLDVERT_TEX_4_NRM_3 = 0x8,
		MTL_WORLDVERT_TEX_5_NRM_1 = 0x9,
		MTL_WORLDVERT_TEX_5_NRM_2 = 0xA,
		MTL_WORLDVERT_TEX_5_NRM_3 = 0xB,
	};

	struct MaterialTechniqueSet
	{
		char* name;
		MaterialWorldVertexFormat worldVertFormat;
		bool hasBeenUploaded;
		char unused[1];
		MaterialTechniqueSet* remappedTechniqueSet;
		MaterialTechnique* techniques[34];
	};

	struct MaterialGameFlagsFields
	{
		unsigned char unk1 : 1; // 0x1
		unsigned char addShadowToPrimaryLight : 1; // 0x2
		unsigned char isFoliageRequiresGroundLighting : 1; // 0x4
		unsigned char unk4 : 1; // 0x8
		unsigned char unk5 : 1; // 0x10
		unsigned char unk6 : 1; // 0x20
		unsigned char MTL_GAMEFLAG_CASTS_SHADOW : 1; // 0x40
		unsigned char unkNeededForSModelDisplay : 1; // 0x80
	};

	union MaterialGameFlags
	{
		MaterialGameFlagsFields fields;
		unsigned char packed;
	};

#pragma pack(push, 4)
	struct MaterialInfo
	{
		const char* name;
		MaterialGameFlags gameFlags;
		char sortKey;
		char textureAtlasRowCount;
		char textureAtlasColumnCount;
		GfxDrawSurf drawSurf;
		unsigned int surfaceTypeBits;
		unsigned __int16 hashIndex;
	};
#pragma pack(pop)

	enum MaterialTechniqueType
	{
		TECHNIQUE_DEPTH_PREPASS = 0x0,
		TECHNIQUE_BUILD_FLOAT_Z = 0x1,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
		TECHNIQUE_UNLIT = 0x4,
		TECHNIQUE_EMISSIVE = 0x5,
		TECHNIQUE_EMISSIVE_SHADOW = 0x6,
		TECHNIQUE_LIT_BEGIN = 0x7,
		TECHNIQUE_LIT = 0x7,
		TECHNIQUE_LIT_SUN = 0x8,
		TECHNIQUE_LIT_SUN_SHADOW = 0x9,
		TECHNIQUE_LIT_SPOT = 0xA,
		TECHNIQUE_LIT_SPOT_SHADOW = 0xB,
		TECHNIQUE_LIT_OMNI = 0xC,
		TECHNIQUE_LIT_OMNI_SHADOW = 0xD,
		TECHNIQUE_LIT_INSTANCED = 0xE,
		TECHNIQUE_LIT_INSTANCED_SUN = 0xF,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW = 0x10,
		TECHNIQUE_LIT_INSTANCED_SPOT = 0x11,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW = 0x12,
		TECHNIQUE_LIT_INSTANCED_OMNI = 0x13,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW = 0x14,
		TECHNIQUE_LIT_END = 0x15,
		TECHNIQUE_LIGHT_SPOT = 0x15,
		TECHNIQUE_LIGHT_OMNI = 0x16,
		TECHNIQUE_LIGHT_SPOT_SHADOW = 0x17,
		TECHNIQUE_FAKELIGHT_NORMAL = 0x18,
		TECHNIQUE_FAKELIGHT_VIEW = 0x19,
		TECHNIQUE_SUNLIGHT_PREVIEW = 0x1A,
		TECHNIQUE_CASE_TEXTURE = 0x1B,
		TECHNIQUE_WIREFRAME_SOLID = 0x1C,
		TECHNIQUE_WIREFRAME_SHADED = 0x1D,
		TECHNIQUE_SHADOWCOOKIE_CASTER = 0x1E,
		TECHNIQUE_SHADOWCOOKIE_RECEIVER = 0x1F,
		TECHNIQUE_DEBUG_BUMPMAP = 0x20,
		TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x21,
		//TECHNIQUE_COUNT = 0x22
	};

	enum MaterialTextureSource : unsigned int
	{
		TEXTURE_SRC_CODE_BLACK = 0x0,
		TEXTURE_SRC_CODE_WHITE = 0x1,
		TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP = 0x2,
		TEXTURE_SRC_CODE_MODEL_LIGHTING = 0x3,
		TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY = 0x4,
		TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY = 0x5,
		TEXTURE_SRC_CODE_SHADOWCOOKIE = 0x6,
		TEXTURE_SRC_CODE_SHADOWMAP_SUN = 0x7,
		TEXTURE_SRC_CODE_SHADOWMAP_SPOT = 0x8,
		TEXTURE_SRC_CODE_FEEDBACK = 0x9,
		TEXTURE_SRC_CODE_RESOLVED_POST_SUN = 0xA,
		TEXTURE_SRC_CODE_RESOLVED_SCENE = 0xB,
		TEXTURE_SRC_CODE_POST_EFFECT_0 = 0xC,
		TEXTURE_SRC_CODE_POST_EFFECT_1 = 0xD,
		TEXTURE_SRC_CODE_SKY = 0xE,
		TEXTURE_SRC_CODE_LIGHT_ATTENUATION = 0xF,
		TEXTURE_SRC_CODE_DYNAMIC_SHADOWS = 0x10,
		TEXTURE_SRC_CODE_OUTDOOR = 0x11,
		TEXTURE_SRC_CODE_FLOATZ = 0x12,
		TEXTURE_SRC_CODE_PROCESSED_FLOATZ = 0x13,
		TEXTURE_SRC_CODE_RAW_FLOATZ = 0x14,
		TEXTURE_SRC_CODE_CASE_TEXTURE = 0x15,
		TEXTURE_SRC_CODE_CINEMATIC_Y = 0x16,
		TEXTURE_SRC_CODE_CINEMATIC_CR = 0x17,
		TEXTURE_SRC_CODE_CINEMATIC_CB = 0x18,
		TEXTURE_SRC_CODE_CINEMATIC_A = 0x19,
		TEXTURE_SRC_CODE_REFLECTION_PROBE = 0x1A,
		TEXTURE_SRC_CODE_COUNT = 0x1B,
	};

	struct infoParm_t
	{
		const char* name;
		int clearSolid;
		int surfaceFlags;
		int contents;
		int toolFlags;
	};

	enum StateFlags : unsigned char {
		STATE_FLAG_CULL_BACK = 0x1,
		STATE_FLAG_CULL_FRONT = 0x2,
		STATE_FLAG_DECAL = 0x4,
		STATE_FLAG_WRITES_DEPTH = 0x8,
		STATE_FLAG_USES_DEPTH_BUFFER = 0x10,
		STATE_FLAG_USES_STENCIL_BUFFER = 0x20,
		STATE_FLAG_CULL_BACK_SHADOW = 0x40,
		STATE_FLAG_CULL_FRONT_SHADOW = 0x80
	};
	enum MaterialStateSet
	{
		MTL_STATE_SET_ALPHA_TEST = 0x0,
		MTL_STATE_SET_BLEND_FUNC_RGB = 0x1,
		MTL_STATE_SET_BLEND_FUNC_ALPHA = 0x2,
		MTL_STATE_SET_CULL_FACE = 0x3,
		MTL_STATE_SET_DEPTH_TEST = 0x4,
		MTL_STATE_SET_DEPTH_WRITE = 0x5,
		MTL_STATE_SET_COLOR_WRITE = 0x6,
		MTL_STATE_SET_POLYGON_OFFSET = 0x7,
		MTL_STATE_SET_STENCIL = 0x8,
		MTL_STATE_SET_WIREFRAME = 0x9,
		MTL_STATE_SET_COUNT = 0xA,
	};

	struct Material
	{
		MaterialInfo info;
		char stateBitsEntry[34];
		char textureCount;
		char constantCount;
		char stateBitsCount;
		StateFlags stateFlags;
		char cameraRegion;
		MaterialTechniqueSet* techniqueSet;
		MaterialTextureDef* textureTable;
		MaterialConstantDef* constantTable;
		GfxStateBits* stateBitsTable;
	};

	struct cplane_s
	{
		float normal[3];
		float dist;
		char type;
		char signbits;
		char pad[2];
	};

#pragma pack(push, 2)
	struct cbrushside_t
	{
		cplane_s* plane;
		unsigned int materialNum;
		__int16 firstAdjacentSideOffset;
		char edgeCount;
	};
#pragma pack(pop)

	struct DObjAnimMat
	{
		float quat[4];
		float trans[3];
		float transWeight;
	};

	struct XSurfaceVertexInfo
	{
		unsigned short vertCount[4];
		unsigned short* vertsBlend;
	};


	struct GameWorldMp
	{
		const char* name;
	};

	union GfxColor
	{
		unsigned int packed;
		unsigned char array[4];
	};

	union PackedTexCoords
	{
		unsigned int packed;
	};

	union PackedUnitVec
	{
		unsigned int packed;
		char array[4];
	};

	struct GfxPackedVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		PackedTexCoords texCoord;
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct XSurfaceCollisionAabb
	{
		unsigned short mins[3];
		unsigned short maxs[3];
	};

	struct XSurfaceCollisionNode
	{
		XSurfaceCollisionAabb aabb;
		unsigned short childBeginIndex;
		unsigned short childCount;
	};

	struct XSurfaceCollisionLeaf
	{
		unsigned short triangleBeginIndex;
	};

	struct XSurfaceCollisionTree
	{
		float trans[3];
		float scale[3];
		unsigned int nodeCount;
		XSurfaceCollisionNode* nodes;
		unsigned int leafCount;
		XSurfaceCollisionLeaf* leafs;
	};

	struct XRigidVertList
	{
		unsigned short boneOffset;
		unsigned short vertCount;
		unsigned short triOffset;
		unsigned short triCount;
		XSurfaceCollisionTree* collisionTree;
	};

	struct XSurface
	{
		unsigned char tileMode;
		bool deformed;
		unsigned __int16 vertCount;
		unsigned __int16 triCount;
		char zoneHandle;
		unsigned __int16 baseTriIndex;
		unsigned __int16 baseVertIndex;
		unsigned __int16* triIndices;
		XSurfaceVertexInfo vertInfo;
		GfxPackedVertex* verts0;
		unsigned int vertListCount;
		XRigidVertList* vertList;
		int partBits[4];
	};

	struct BrushWrapper
	{
		float mins[3];
		int contents;
		float maxs[3];
		unsigned int numsides;
		cbrushside_t* sides;
		__int16 axialMaterialNum[2][3];
		char* baseAdjacentSide;
		__int16 firstAdjacentSideOffsets[2][3];
		char edgeCount[2][3];
		int totalEdgeCount;
		cplane_s* planes;
	};

	struct PhysMass
	{
		float centerOfMass[3];
		float momentsOfInertia[3];
		float productsOfInertia[3];
	};

	struct PhysGeomInfo
	{
		BrushWrapper* brush;
		int type;
		float orientation[3][3];
		float offset[3];
		float halfLengths[3];
	};

	struct PhysGeomList
	{
		unsigned int count;
		PhysGeomInfo* geoms;
		PhysMass mass;
	};

	struct XBoneInfo
	{
		float bounds[2][3];
		float offset[3];
		float radiusSquared;
	};

	struct XModelHighMipBounds
	{
		float mins[3];
		float maxs[3];
	};

	struct XModelStreamInfo
	{
		XModelHighMipBounds* highMipBounds;
	};

	struct XModelCollTri_s
	{
		float plane[4];
		float svec[4];
		float tvec[4];
	};

	struct XModelCollSurf_s
	{
		XModelCollTri_s* collTris;
		int numCollTris;
		float mins[3];
		float maxs[3];
		int boneIdx;
		int contents;
		int surfFlags;
	};

	struct XModelLodInfo
	{
		float dist;
		unsigned __int16 numsurfs;
		unsigned __int16 surfIndex;
		int partBits[4];
		char lod;
		char smcIndexPlusOne;
		char smcAllocBits;
		char unused;
	};

#pragma pack(push, 4)
	struct PhysPreset
	{
		const char* name;
		int type;
		float mass;
		float bounce;
		float friction;
		float bulletForceScale;
		float explosiveForceScale;
		const char* sndAliasPrefix;
		float piecesSpreadFraction;
		float piecesUpwardVelocity;
		char tempDefaultToCylinder;
	};
#pragma pack(pop)

	struct XModel
	{
		const char* name;
		char numBones;
		char numRootBones;
		unsigned char numsurfs;
		char lodRampType;
		unsigned __int16* boneNames;
		unsigned char* parentList;
		__int16* quats;
		float* trans;
		unsigned char* partClassification;
		DObjAnimMat* baseMat;
		XSurface* surfs;
		Material** materialHandles;
		XModelLodInfo lodInfo[4];
		XModelCollSurf_s* collSurfs;
		int numCollSurfs;
		int contents;
		XBoneInfo* boneInfo;
		float radius;
		float mins[3];
		float maxs[3];
		unsigned __int16 numLods;
		unsigned __int16 collLod;
		XModelStreamInfo streamInfo;
		int memUsage;
		char flags;
		bool bad;
		PhysPreset* physPreset;
		PhysGeomList* physGeoms;
	};

	union XAnimIndices
	{
		char* _1;
		unsigned __int16* _2;
		void* data;
	};

	union XAnimDynamicFrames
	{
		char(*_1)[3];
		unsigned __int16(*_2)[3];
	};

	union XAnimDynamicIndices
	{
		char _1[1];
		unsigned __int16 _2[1];
	};

#pragma pack(push, 4)
	struct XAnimPartTransFrames
	{
		float mins[3];
		float size[3];
		XAnimDynamicFrames frames;
		XAnimDynamicIndices indices;
	};

	union XAnimPartTransData
	{
		XAnimPartTransFrames frames;
		float frame0[3];
	};

	struct XAnimPartTrans
	{
		unsigned __int16 size;
		char smallTrans;
		__declspec(align(2)) XAnimPartTransData u;
	};

	struct XAnimDeltaPartQuatDataFrames
	{
		__int16(*frames)[2];
		XAnimDynamicIndices indices;
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames;
		__int16 frame0[2];
	};

	struct XAnimDeltaPartQuat
	{
		unsigned __int16 size;
		__declspec(align(4)) XAnimDeltaPartQuatData u;
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans* trans;
		XAnimDeltaPartQuat* quat;
	};

	struct XAnimNotifyInfo
	{
		ScriptString name;
		float time;
	};

#ifdef __cplusplus
	enum XAnimPartType
	{
		PART_TYPE_NO_QUAT = 0x0,
		PART_TYPE_SIMPLE_QUAT = 0x1,
		PART_TYPE_NORMAL_QUAT = 0x2,
		PART_TYPE_PRECISION_QUAT = 0x3,
		PART_TYPE_SIMPLE_QUAT_NO_SIZE = 0x4,
		PART_TYPE_NORMAL_QUAT_NO_SIZE = 0x5,
		PART_TYPE_PRECISION_QUAT_NO_SIZE = 0x6,
		PART_TYPE_SMALL_TRANS = 0x7,
		PART_TYPE_TRANS = 0x8,
		PART_TYPE_TRANS_NO_SIZE = 0x9,
		PART_TYPE_NO_TRANS = 0xA,
		PART_TYPE_ALL = 0xB,
		PART_TYPE_COUNT = 0xC,
	};
#endif

#pragma pack(pop)

	struct XAnimParts
	{
		const char* name;
		unsigned __int16 dataByteCount;
		unsigned __int16 dataShortCount;
		unsigned __int16 dataIntCount;
		unsigned __int16 randomDataByteCount;
		unsigned __int16 randomDataIntCount;
		unsigned __int16 numframes;
		char bLoop;
		char bDelta;
		char boneCount[12];
		char notifyCount;
		char assetType;
		unsigned int randomDataShortCount;
		unsigned int indexCount;
		float framerate;
		float frequency;
		ScriptString* names;
		char* dataByte;
		__int16* dataShort;
		int* dataInt;
		__int16* randomDataShort;
		char* randomDataByte;
		int* randomDataInt;
		XAnimIndices indices;
		XAnimNotifyInfo* notify;
		XAnimDeltaPart* deltaPart;
	};

	struct GfxStreamingAabbTree
	{
		unsigned __int16 firstItem;
		unsigned __int16 itemCount;
		unsigned __int16 firstChild;
		unsigned __int16 childCount;
		float mins[3];
		float maxs[3];
	};

	struct GfxWorldStreamInfo
	{
		int aabbTreeCount;
		// 			GfxStreamingAabbTree *aabbTrees;
		// 			int leafRefCount;
		// 			int *leafRefs;
	};

	struct GfxWorldVertex
	{
		float xyz[3];
		float binormalSign;
		GfxColor color;
		float texCoord[2];
		float lmapCoord[2];
		PackedUnitVec normal;
		PackedUnitVec tangent;
	};

	struct GfxWorldVertexData
	{
		GfxWorldVertex* vertices;
		void/*IDirect3DVertexBuffer9*/* worldVb;
	};

	struct GfxWorldVertexLayerData
	{
		char* data;
		void/*IDirect3DVertexBuffer9*/* layerVb;
	};

	struct SunLightParseParams
	{
		char name[64];
		float ambientScale;
		float ambientColor[3];
		float diffuseFraction;
		float sunLight;
		float sunColor[3];
		float diffuseColor[3];
		char diffuseColorHasBeenSet;
		float angles[3];
	};

	struct __declspec(align(4)) GfxLightImage
	{
		GfxImage* image;
		char samplerState;
	};

	struct GfxLightDef
	{
		const char* name;
		GfxLightImage attenuation;
		int lmapLookupStart;
	};

	struct GfxLight
	{
		char type;
		char canUseShadowMap;
		char unused[2];
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
		unsigned int spotShadowIndex;
		GfxLightDef* def;
	};

	struct GfxReflectionProbe
	{
		float origin[3];
		GfxImage* reflectionImage;
	};

	struct GfxWorldDpvsPlanes
	{
		int cellCount;
		cplane_s* planes;
		unsigned __int16* nodes;
		unsigned int* sceneEntCellBits;
	};

	struct GfxAabbTree
	{
		float mins[3];
		float maxs[3];
		unsigned __int16 childCount;
		unsigned __int16 surfaceCount;
		unsigned __int16 startSurfIndex;
		unsigned __int16 surfaceCountNoDecal;
		unsigned __int16 startSurfIndexNoDecal;
		unsigned __int16 smodelIndexCount;
		unsigned __int16* smodelIndexes;
		int childrenOffset;
	};

	struct GfxPortal;

	struct GfxPortalWritable
	{
		char isQueued;
		char isAncestor;
		char recursionDepth;
		char hullPointCount;
		float(*hullPoints)[2];
		GfxPortal* queuedParent;
	};

	struct DpvsPlane
	{
		float coeffs[4];
		char side[3];
		char pad;
	};

	struct GfxCell;

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		GfxCell* cell;
		float(*vertices)[3];
		char vertexCount;
		float hullAxis[2][3];
	};

	struct GfxCell
	{
		float mins[3];
		float maxs[3];
		int aabbTreeCount;
		GfxAabbTree* aabbTree;
		int portalCount;
		GfxPortal* portals;
		int cullGroupCount;
		int* cullGroups;
		unsigned char reflectionProbeCount;
		unsigned char* reflectionProbes;
	};

	struct GfxLightmapArray
	{
		GfxImage* primary;
		GfxImage* secondary;
	};

	struct GfxLightGridEntry
	{
		unsigned __int16 colorsIndex;
		char primaryLightIndex;
		char needsTrace;
	};

	struct GfxLightGridColors
	{
		char rgb[56][3];
	};

	struct GfxLightGrid
	{
		char hasLightRegions;
		unsigned int sunPrimaryLightIndex;
		unsigned __int16 mins[3];
		unsigned __int16 maxs[3];
		unsigned int rowAxis;
		unsigned int colAxis;
		unsigned __int16* rowDataStart;
		unsigned int rawRowDataSize;
		char* rawRowData;
		unsigned int entryCount;
		GfxLightGridEntry* entries;
		unsigned int colorCount;
		GfxLightGridColors* colors;
	};

	struct GfxBrushModelWritable
	{
		float mins[3];
		float maxs[3];
	};

	struct __declspec(align(4)) GfxBrushModel
	{
		GfxBrushModelWritable writable;
		float bounds[2][3];
		unsigned __int16 surfaceCount;
		unsigned __int16 startSurfIndex;
		unsigned __int16 surfaceCountNoDecal;
	};

	struct MaterialMemory
	{
		Material* material;
		int memory;
	};

	struct sunflare_t
	{
		char hasValidData;
		Material* spriteMaterial;
		Material* flareMaterial;
		float spriteSize;
		float flareMinSize;
		float flareMinDot;
		float flareMaxSize;
		float flareMaxDot;
		float flareMaxAlpha;
		int flareFadeInTime;
		int flareFadeOutTime;
		float blindMinDot;
		float blindMaxDot;
		float blindMaxDarken;
		int blindFadeInTime;
		int blindFadeOutTime;
		float glareMinDot;
		float glareMaxDot;
		float glareMaxLighten;
		int glareFadeInTime;
		int glareFadeOutTime;
		float sunFxPosition[3];
	};

	struct XModelDrawInfo
	{
		unsigned __int16 lod;
		unsigned __int16 surfId;
	};

	struct GfxSceneDynModel
	{
		XModelDrawInfo info;
		unsigned __int16 dynEntId;
	};

	struct BModelDrawInfo
	{
		unsigned __int16 surfId;
	};

	struct GfxSceneDynBrush
	{
		BModelDrawInfo info;
		unsigned __int16 dynEntId;
	};

	struct GfxShadowGeometry
	{
		unsigned __int16 surfaceCount;
		unsigned __int16 smodelCount;
		unsigned __int16* sortedSurfIndex;
		unsigned __int16* smodelIndex;
	};

	struct GfxLightRegionAxis
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct GfxLightRegionHull
	{
		float kdopMidPoint[9];
		float kdopHalfSize[9];
		unsigned int axisCount;
		GfxLightRegionAxis* axis;
	};

	struct GfxLightRegion
	{
		unsigned int hullCount;
		GfxLightRegionHull* hulls;
	};

	struct GfxStaticModelInst
	{
		float mins[3];
		float maxs[3];
		GfxColor groundLighting;
	};

	struct srfTriangles_t
	{
		int vertexLayerData;
		int firstVertex;
		unsigned __int16 vertexCount;
		unsigned __int16 triCount;
		int baseIndex;
	};

	struct GfxSurface
	{
		srfTriangles_t tris;
		Material* material;
		unsigned char lightmapIndex;
		unsigned char reflectionProbeIndex;
		unsigned char primaryLightIndex;
		unsigned char flags;
		float bounds[2][3];
	};

	struct GfxCullGroup
	{
		float mins[3];
		float maxs[3];
		int surfaceCount;
		int startSurfIndex;
	};

	struct GfxPackedPlacement
	{
		float origin[3];
		vec3_t axis[3];
		float scale;
	};

	struct __declspec(align(4)) GfxStaticModelDrawInst
	{
		float cullDist;
		GfxPackedPlacement placement;
		XModel* model;
		unsigned __int16 smodelCacheIndex[4];
		unsigned char reflectionProbeIndex;
		unsigned char primaryLightIndex;
		unsigned __int16 lightingHandle;
		unsigned char flags;
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int staticSurfaceCountNoDecal;
		unsigned int litSurfsBegin;
		unsigned int litSurfsEnd;
		unsigned int decalSurfsBegin;
		unsigned int decalSurfsEnd;
		unsigned int emissiveSurfsBegin;
		unsigned int emissiveSurfsEnd;
		unsigned int smodelVisDataCount;
		unsigned int surfaceVisDataCount;
		unsigned char* smodelVisData[3];
		unsigned char* surfaceVisData[3];
		unsigned int* lodData;
		unsigned __int16* sortedSurfIndex;
		GfxStaticModelInst* smodelInsts;
		GfxSurface* surfaces;
		GfxCullGroup* cullGroups;
		GfxStaticModelDrawInst* smodelDrawInsts;
		GfxDrawSurf* surfaceMaterials;
		unsigned int* surfaceCastsSunShadow;
		volatile int usageCount;
	};

	struct GfxWorldDpvsDynamic
	{
		unsigned int dynEntClientWordCount[2];
		unsigned int dynEntClientCount[2];
		unsigned int* dynEntCellBits[2];
		char* dynEntVisData[2][3];
	};

	struct GfxWorld
	{
		const char* name;
		const char* baseName;
		int planeCount;
		int nodeCount;
		int indexCount;
		unsigned __int16* indices;
		int surfaceCount;
		GfxWorldStreamInfo streamInfo;
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		char skySamplerState;
		unsigned int vertexCount;
		GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		GfxWorldVertexLayerData vld;
		SunLightParseParams sunParse;
		GfxLight* sunLight;
		float sunColorFromBsp[3];
		unsigned int sunPrimaryLightIndex;
		unsigned int primaryLightCount;
		int cullGroupCount;
		unsigned int reflectionProbeCount;
		GfxReflectionProbe* reflectionProbes;
		GfxTexture* reflectionProbeTextures;
		GfxWorldDpvsPlanes dpvsPlanes;
		int cellBitsCount;
		GfxCell* cells;
		int lightmapCount;
		GfxLightmapArray* lightmaps;
		GfxLightGrid lightGrid;
		GfxTexture* lightmapPrimaryTextures;
		GfxTexture* lightmapSecondaryTextures;
		int modelCount;
		GfxBrushModel* models;
		float mins[3];
		float maxs[3];
		unsigned int checksum;
		int materialMemoryCount;
		MaterialMemory* materialMemory;
		sunflare_t sun;
		float outdoorLookupMatrix[4][4];
		GfxImage* outdoorImage;
		unsigned int* cellCasterBits;
		GfxSceneDynModel* sceneDynModel;
		GfxSceneDynBrush* sceneDynBrush;
		unsigned int* primaryLightEntityShadowVis;
		unsigned int* primaryLightDynEntShadowVis[2];
		char* nonSunPrimaryLightForModelDynEnt;
		GfxShadowGeometry* shadowGeom;
		GfxLightRegion* lightRegion;
		GfxWorldDpvsStatic dpvs;
		GfxWorldDpvsDynamic dpvsDyn;
	};

	struct cStaticModelWritable
	{
		unsigned __int16 nextModelInWorldSector;
	};

	struct cStaticModel_s
	{
		cStaticModelWritable writable;
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float absmin[3];
		float absmax[3];
	};

	struct dmaterial_t
	{
		char material[64];
		int surfaceFlags;
		int contentFlags;
	};

	struct cNode_t
	{
		cplane_s* plane;
		__int16 children[2];
	};

#pragma pack(push, 4)
	struct cLeaf_t
	{
		unsigned __int16 firstCollAabbIndex;
		unsigned __int16 collAabbCount;
		int brushContents;
		int terrainContents;
		float mins[3];
		float maxs[3];
		int leafBrushNode;
		__int16 cluster;
	};
#pragma pack(pop)

	struct cLeafBrushNodeLeaf_t
	{
		unsigned __int16* brushes;
	};

	struct cLeafBrushNodeChildren_t
	{
		float dist;
		float range;
		unsigned __int16 childOffset[2];
	};

	union cLeafBrushNodeData_t
	{
		cLeafBrushNodeLeaf_t leaf;
		cLeafBrushNodeChildren_t children;
	};

	struct cLeafBrushNode_s
	{
		char axis;
		__int16 leafBrushCount;
		int contents;
		cLeafBrushNodeData_t data;
	};

	struct CollisionBorder
	{
		float distEq[3];
		float zBase;
		float zSlope;
		float start;
		float length;
	};

	struct CollisionPartition
	{
		char triCount;
		char borderCount;
		int firstTri;
		CollisionBorder* borders;
	};

	union CollisionAabbTreeIndex
	{
		int firstChildIndex;
		int partitionIndex;
	};

	struct CollisionAabbTree
	{
		float origin[3];
		float halfSize[3];
		unsigned __int16 materialIndex;
		unsigned __int16 childCount;
		CollisionAabbTreeIndex u;
	};

	/* 860 */
	struct cmodel_t
	{
		float mins[3];
		float maxs[3];
		float radius;
		cLeaf_t leaf;
	};

	/* 861 */
#pragma pack(push, 16)
	struct cbrush_t
	{
		float mins[3];
		int contents;
		float maxs[3];
		unsigned int numsides;
		cbrushside_t* sides;
		__int16 axialMaterialNum[2][3];
		char* baseAdjacentSide;
		__int16 firstAdjacentSideOffsets[2][3];
		char edgeCount[2][3];
		char pad[8];
	};
#pragma pack(pop)

	struct Bounds
	{
		vec3_t midPoint;
		vec3_t halfSize;
	};

	struct TriggerModel
	{
		int contents;
		unsigned __int16 hullCount;
		unsigned __int16 firstHull;
	};

	struct TriggerHull
	{
		Bounds bounds;
		int contents;
		unsigned __int16 slabCount;
		unsigned __int16 firstSlab;
	};

	struct TriggerSlab
	{
		float dir[3];
		float midPoint;
		float halfSize;
	};

	struct MapTriggers
	{
		unsigned int count;
		TriggerModel* models;
		unsigned int hullCount;
		TriggerHull* hulls;
		unsigned int slabCount;
		TriggerSlab* slabs;
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		//MapTriggers trigger;
		// this goes on for a while but we don't need any of it
	};

	struct GfxPlacement
	{
		float quat[4];
		float origin[3];
	};

	struct FxEffectDef_Placeholder
	{
		const char* name;
	};

	struct XModelPiece
	{
		XModel* model;
		float offset[3];
	};

	struct XModelPieces
	{
		const char* name;
		int numpieces;
		XModelPiece* pieces;
	};


	struct FxSpawnDefLooping
	{
		int intervalMsec;
		int count;
	};

	struct FxIntRange
	{
		int base;
		int amplitude;
	};

	struct FxSpawnDefOneShot
	{
		FxIntRange count;
	};

	union FxSpawnDef
	{
		FxSpawnDefLooping looping;
		FxSpawnDefOneShot oneShot;
	};

	struct FxFloatRange
	{
		float base;
		float amplitude;
	};

	struct FxElemAtlas
	{
		char behavior;
		char index;
		char fps;
		char loopCount;
		char colIndexBits;
		char rowIndexBits;
		__int16 entryCount;
	};

	struct FxElemVec3Range
	{
		float base[3];
		float amplitude[3];
	};

	struct FxElemVelStateInFrame
	{
		FxElemVec3Range velocity;
		FxElemVec3Range totalDelta;
	};

	const struct FxElemVelStateSample
	{
		FxElemVelStateInFrame local;
		FxElemVelStateInFrame world;
	};

	struct FxElemVisualState
	{
		char color[4];
		float rotationDelta;
		float rotationTotal;
		float size[2];
		float scale;
	};

	const struct FxElemVisStateSample
	{
		FxElemVisualState base;
		FxElemVisualState amplitude;
	};


	struct FxEffectDef;
	union FxEffectDefRef
	{
		FxEffectDef* handle;
		const char* name;
	};

	union FxElemVisuals
	{
		const void* anonymous;
		Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
	};

	struct FxElemMarkVisuals
	{
		Material* materials[2];
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};

	struct FxTrailVertex
	{
		float pos[2];
		float normal[2];
		float texCoord;
	};

	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		int splitDist;
		int vertCount;
		FxTrailVertex* verts;
		int indCount;
		unsigned __int16* inds;
	};

	enum FxElemType : char
	{
		FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
		FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
		FX_ELEM_TYPE_TAIL = 0x2,
		FX_ELEM_TYPE_TRAIL = 0x3,
		FX_ELEM_TYPE_CLOUD = 0x4,
		FX_ELEM_TYPE_MODEL = 0x5,
		FX_ELEM_TYPE_OMNI_LIGHT = 0x6,
		FX_ELEM_TYPE_SPOT_LIGHT = 0x7,
		FX_ELEM_TYPE_SOUND = 0x8,
		FX_ELEM_TYPE_DECAL = 0x9,
		FX_ELEM_TYPE_RUNNER = 0xA,
		FX_ELEM_TYPE_COUNT = 0xB,
		FX_ELEM_TYPE_LAST_SPRITE = 0x3,
		FX_ELEM_TYPE_LAST_DRAWN = 0x7,
	};

	const struct FxElemDef
	{
		int flags;
		FxSpawnDef spawn;
		FxFloatRange spawnRange;
		FxFloatRange fadeInRange;
		FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		FxIntRange spawnDelayMsec;
		FxIntRange lifeSpanMsec;
		FxFloatRange spawnOrigin[3];
		FxFloatRange spawnOffsetRadius;
		FxFloatRange spawnOffsetHeight;
		FxFloatRange spawnAngles[3];
		FxFloatRange angularVelocity[3];
		FxFloatRange initialRotation;
		FxFloatRange gravity;
		FxFloatRange reflectionFactor;
		FxElemAtlas atlas;
		FxElemType elemType;
		char visualCount;
		char velIntervalCount;
		char visStateIntervalCount;
		FxElemVelStateSample* velSamples;
		FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		float collMins[3];
		float collMaxs[3];
		FxEffectDefRef effectOnImpact;
		FxEffectDefRef effectOnDeath;
		FxEffectDefRef effectEmitted;
		FxFloatRange emitDist;
		FxFloatRange emitDistVariance;
		FxTrailDef* trailDef;
		char sortOrder;
		char lightingFrac;
		char useItemClip;
		char unused[1];
	};

	struct FxEffectDef
	{
		const char* name;
		int flags;
		int totalSize;
		int msecLoopingLife;
		int elemDefCountLooping;
		int elemDefCountOneShot;
		int elemDefCountEmission;
		FxElemDef* elemDefs;
	};

	struct DynEntityDef
	{
		int type;
		GfxPlacement pose;
		XModel* xModel;
		unsigned __int16 brushModel;
		unsigned __int16 physicsBrushModel;
		FxEffectDef* destroyFx;
		XModelPieces* destroyPieces;
		PhysPreset* physPreset;
		int health;
		PhysMass mass;
		int contents;
	};

	struct clipMap_t
	{
		const char* name;
		int isInUse;
		int planeCount;
		cplane_s* planes;
		unsigned int numStaticModels;
		cStaticModel_s* staticModelList;
		unsigned int numMaterials;
		dmaterial_t* materials;
		unsigned int numBrushSides;
		cbrushside_t* brushsides;
		unsigned int numBrushEdges;
		char* brushEdges;
		unsigned int numNodes;
		cNode_t* nodes;
		unsigned int numLeafs;
		cLeaf_t* leafs;
		unsigned int leafbrushNodesCount;
		cLeafBrushNode_s* leafbrushNodes;
		unsigned int numLeafBrushes;
		unsigned __int16* leafbrushes;
		unsigned int numLeafSurfaces;
		unsigned int* leafsurfaces;
		unsigned int vertCount;
		float(*verts)[3];
		int triCount;
		unsigned __int16* triIndices;
		char* triEdgeIsWalkable;
		int borderCount;
		CollisionBorder* borders;
		int partitionCount;
		CollisionPartition* partitions;
		int aabbTreeCount;
		CollisionAabbTree* aabbTrees;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		unsigned __int16 numBrushes;
		cbrush_t* brushes;
		int numClusters;
		int clusterBytes;
		char* visibility;
		int vised;
		MapEnts* mapEnts;
		cbrush_t* box_brush;
		cmodel_t box_model;
		unsigned __int16 dynEntCount[2];
		DynEntityDef* dynEntDefList[2];
		/*DynEntityPose*/ void* dynEntPoseList[2];
		/*DynEntityClient*/ void* dynEntClientList[2];
		/*DynEntityColl*/ void* dynEntCollList[2];
		unsigned int checksum;
	};

	struct RawFile
	{
		const char* name;
		int len;
		char* buffer;
	};

	struct ComPrimaryLight
	{
		char type;
		char canUseShadowMap;
		char exponent;
		char unused;
		float color[3];
		float dir[3];
		float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		float cosHalfFovExpanded;
		float rotationLimit;
		float translationLimit;
		const char* defName;
	};

	struct ComWorld
	{
		const char* name;
		int isInUse;
		unsigned int primaryLightCount;
		ComPrimaryLight* primaryLights;
	};

	struct _AILSOUNDINFO
	{
		int format;
		const void* data_ptr;
		unsigned int data_len;
		unsigned int rate;
		int bits;
		int channels;
		unsigned int samples;
		unsigned int block_size;
		const void* initial_ptr;
	};

	struct MssSound
	{
		_AILSOUNDINFO info;
		char* data;
	};

	struct LoadedSound
	{
		const char* name;
		MssSound sound;
	};

	struct StreamFileNameRaw
	{
		const char* dir;
		const char* name;
	};

	union StreamFileInfo
	{
		StreamFileNameRaw raw;
	};

	struct StreamFileName
	{
		StreamFileInfo info;
	};

	struct StreamedSound
	{
		StreamFileName filename;
	};

	union SoundFileRef
	{
		LoadedSound* loadSnd;
		StreamedSound streamSnd;
	};

	struct SoundFile
	{
		char type;
		char exists;
		SoundFileRef u;
	};

	struct MSSSpeakerLevels
	{
		int speaker;
		int numLevels;
		float levels[2];
	};

	struct MSSChannelMap
	{
		unsigned int speakerCount;
		MSSSpeakerLevels speakers[6];
	};

	struct SpeakerMap
	{
		bool isDefault;
		const char* name;
		MSSChannelMap channelMaps[2][2];
	};

	struct SndCurve
	{
		const char* filename;
		int knotCount;
		float knots[8][2];
	};

	const struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		SoundFile* soundFile;
		int sequence;
		float volMin;
		float volMax;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		int flags;
		float slavePercentage;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
		SndCurve* volumeFalloffCurve;
		float envelopMin;
		float envelopMax;
		float envelopPercentage;
		SpeakerMap* speakerMap;
	};

	struct snd_alias_list_t
	{
		const char* aliasName;
		snd_alias_t* head;
		unsigned int count;
	};

	struct LocalizeEntry
	{
		const char* value;
		const char* name;
	};


	/* 317 */
	enum guidedMissileType_t
	{
		MISSILE_GUIDANCE_NONE = 0x0,
		MISSILE_GUIDANCE_SIDEWINDER = 0x1,
		MISSILE_GUIDANCE_HELLFIRE = 0x2,
		MISSILE_GUIDANCE_JAVELIN = 0x3,
		MISSILE_GUIDANCE_COUNT = 0x4,
	};

	enum weapType_t
	{
		WEAPTYPE_BULLET = 0x0,
		WEAPTYPE_GRENADE = 0x1,
		WEAPTYPE_PROJECTILE = 0x2,
		WEAPTYPE_BINOCULARS = 0x3,
		WEAPTYPE_NUM = 0x4,
	};

	enum weapClass_t
	{
		WEAPCLASS_RIFLE = 0x0,
		WEAPCLASS_MG = 0x1,
		WEAPCLASS_SMG = 0x2,
		WEAPCLASS_SPREAD = 0x3,
		WEAPCLASS_PISTOL = 0x4,
		WEAPCLASS_GRENADE = 0x5,
		WEAPCLASS_ROCKETLAUNCHER = 0x6,
		WEAPCLASS_TURRET = 0x7,
		WEAPCLASS_NON_PLAYER = 0x8,
		WEAPCLASS_ITEM = 0x9,
		WEAPCLASS_NUM = 0xA,
	};

	enum ImpactType
	{
		IMPACT_TYPE_NONE = 0x0,
		IMPACT_TYPE_BULLET_SMALL = 0x1,
		IMPACT_TYPE_BULLET_LARGE = 0x2,
		IMPACT_TYPE_BULLET_AP = 0x3,
		IMPACT_TYPE_SHOTGUN = 0x4,
		IMPACT_TYPE_GRENADE_BOUNCE = 0x5,
		IMPACT_TYPE_GRENADE_EXPLODE = 0x6,
		IMPACT_TYPE_ROCKET_EXPLODE = 0x7,
		IMPACT_TYPE_PROJECTILE_DUD = 0x8,
		IMPACT_TYPE_COUNT = 0x9,
	};

	enum weapInventoryType_t
	{
		WEAPINVENTORY_PRIMARY = 0x0,
		WEAPINVENTORY_OFFHAND = 0x1,
		WEAPINVENTORY_ITEM = 0x2,
		WEAPINVENTORY_ALTMODE = 0x3,
		WEAPINVENTORYCOUNT = 0x4,
	};

	enum weapFireType_t
	{
		WEAPON_FIRETYPE_FULLAUTO = 0x0,
		WEAPON_FIRETYPE_SINGLESHOT = 0x1,
		WEAPON_FIRETYPE_BURSTFIRE2 = 0x2,
		WEAPON_FIRETYPE_BURSTFIRE3 = 0x3,
		WEAPON_FIRETYPE_BURSTFIRE4 = 0x4,
		WEAPON_FIRETYPECOUNT = 0x5,
	};

	enum weapAnimFiles_t
	{
		WEAP_ANIM_ROOT = 0x0,
		WEAP_ANIM_IDLE = 0x1,
		WEAP_ANIM_EMPTY_IDLE = 0x2,
		WEAP_ANIM_FIRE = 0x3,
		WEAP_ANIM_HOLD_FIRE = 0x4,
		WEAP_ANIM_LASTSHOT = 0x5,
		WEAP_ANIM_RECHAMBER = 0x6,
		WEAP_ANIM_MELEE = 0x7,
		WEAP_ANIM_MELEE_CHARGE = 0x8,
		WEAP_ANIM_RELOAD = 0x9,
		WEAP_ANIM_RELOAD_EMPTY = 0xA,
		WEAP_ANIM_RELOAD_START = 0xB,
		WEAP_ANIM_RELOAD_END = 0xC,
		WEAP_ANIM_RAISE = 0xD,
		WEAP_ANIM_FIRST_RAISE = 0xE,
		WEAP_ANIM_DROP = 0xF,
		WEAP_ANIM_ALT_RAISE = 0x10,
		WEAP_ANIM_ALT_DROP = 0x11,
		WEAP_ANIM_QUICK_RAISE = 0x12,
		WEAP_ANIM_QUICK_DROP = 0x13,
		WEAP_ANIM_EMPTY_RAISE = 0x14,
		WEAP_ANIM_EMPTY_DROP = 0x15,
		WEAP_ANIM_SPRINT_IN = 0x16,
		WEAP_ANIM_SPRINT_LOOP = 0x17,
		WEAP_ANIM_SPRINT_OUT = 0x18,
		WEAP_ANIM_DETONATE = 0x19,
		WEAP_ANIM_NIGHTVISION_WEAR = 0x1A,
		WEAP_ANIM_NIGHTVISION_REMOVE = 0x1B,
		WEAP_ANIM_ADS_FIRE = 0x1C,
		WEAP_ANIM_ADS_LASTSHOT = 0x1D,
		WEAP_ANIM_ADS_RECHAMBER = 0x1E,
		WEAP_ANIM_ADS_UP = 0x1F,
		WEAP_ANIM_ADS_DOWN = 0x20,
		NUM_WEAP_ANIMS = 0x21,
	};

	enum WeapStickinessType
	{
		WEAPSTICKINESS_NONE = 0x0,
		WEAPSTICKINESS_ALL = 0x1,
		WEAPSTICKINESS_GROUND = 0x2,
		WEAPSTICKINESS_GROUND_WITH_YAW = 0x3,
		WEAPSTICKINESS_COUNT = 0x4,
	};

	enum OffhandClass
	{
		OFFHAND_CLASS_NONE = 0x0,
		OFFHAND_CLASS_FRAG_GRENADE = 0x1,
		OFFHAND_CLASS_SMOKE_GRENADE = 0x2,
		OFFHAND_CLASS_FLASH_GRENADE = 0x3,
		OFFHAND_CLASS_COUNT = 0x4,
	};

	enum weapStance_t
	{
		WEAPSTANCE_STAND = 0x0,
		WEAPSTANCE_DUCK = 0x1,
		WEAPSTANCE_PRONE = 0x2,
		WEAPSTANCE_NUM = 0x3,
	};

	enum activeReticleType_t
	{
		VEH_ACTIVE_RETICLE_NONE = 0x0,
		VEH_ACTIVE_RETICLE_PIP_ON_A_STICK = 0x1,
		VEH_ACTIVE_RETICLE_BOUNCING_DIAMOND = 0x2,
		VEH_ACTIVE_RETICLE_COUNT = 0x3,
	};

	enum weaponIconRatioType_t
	{
		WEAPON_ICON_RATIO_1TO1 = 0x0,
		WEAPON_ICON_RATIO_2TO1 = 0x1,
		WEAPON_ICON_RATIO_4TO1 = 0x2,
		WEAPON_ICON_RATIO_COUNT = 0x3,
	};

	enum ammoCounterClipType_t
	{
		AMMO_COUNTER_CLIP_NONE = 0x0,
		AMMO_COUNTER_CLIP_MAGAZINE = 0x1,
		AMMO_COUNTER_CLIP_SHORTMAGAZINE = 0x2,
		AMMO_COUNTER_CLIP_SHOTGUN = 0x3,
		AMMO_COUNTER_CLIP_ROCKET = 0x4,
		AMMO_COUNTER_CLIP_BELTFED = 0x5,
		AMMO_COUNTER_CLIP_ALTWEAPON = 0x6,
		AMMO_COUNTER_CLIP_COUNT = 0x7,
	};

	enum weapOverlayReticle_t
	{
		WEAPOVERLAYRETICLE_NONE = 0x0,
		WEAPOVERLAYRETICLE_CROSSHAIR = 0x1,
		WEAPOVERLAYRETICLE_NUM = 0x2,
	};

	enum WeapOverlayInteface_t
	{
		WEAPOVERLAYINTERFACE_NONE = 0x0,
		WEAPOVERLAYINTERFACE_JAVELIN = 0x1,
		WEAPOVERLAYINTERFACE_TURRETSCOPE = 0x2,
		WEAPOVERLAYINTERFACECOUNT = 0x3,
	};

	enum weapProjExposion_t
	{
		WEAPPROJEXP_GRENADE = 0x0,
		WEAPPROJEXP_ROCKET = 0x1,
		WEAPPROJEXP_FLASHBANG = 0x2,
		WEAPPROJEXP_NONE = 0x3,
		WEAPPROJEXP_DUD = 0x4,
		WEAPPROJEXP_SMOKE = 0x5,
		WEAPPROJEXP_HEAVY = 0x6,
		WEAPPROJEXP_NUM = 0x7,
	};

	enum PenetrateType
	{
		PENETRATE_TYPE_NONE = 0x0,
		PENETRATE_TYPE_SMALL = 0x1,
		PENETRATE_TYPE_MEDIUM = 0x2,
		PENETRATE_TYPE_LARGE = 0x3,
		PENETRATE_TYPE_COUNT = 0x4,
	};


	struct WeaponDef
	{
		const char* szInternalName;
		const char* szDisplayName;
		const char* szOverlayName;
		XModel* gunXModel[16];
		XModel* handXModel;
		const char* szXAnims[33];
		const char* szModeName;
		unsigned __int16 hideTags[8];
		unsigned __int16 notetrackSoundMapKeys[16];
		unsigned __int16 notetrackSoundMapValues[16];
		int playerAnimType;
		weapType_t weapType;
		weapClass_t weapClass;
		PenetrateType penetrateType;
		ImpactType impactType;
		weapInventoryType_t inventoryType;
		weapFireType_t fireType;
		OffhandClass offhandClass;
		weapStance_t stance;
		FxEffectDef* viewFlashEffect;
		FxEffectDef* worldFlashEffect;
		snd_alias_list_t* pickupSound;
		snd_alias_list_t* pickupSoundPlayer;
		snd_alias_list_t* ammoPickupSound;
		snd_alias_list_t* ammoPickupSoundPlayer;
		snd_alias_list_t* projectileSound;
		snd_alias_list_t* pullbackSound;
		snd_alias_list_t* pullbackSoundPlayer;
		snd_alias_list_t* fireSound;
		snd_alias_list_t* fireSoundPlayer;
		snd_alias_list_t* fireLoopSound;
		snd_alias_list_t* fireLoopSoundPlayer;
		snd_alias_list_t* fireStopSound;
		snd_alias_list_t* fireStopSoundPlayer;
		snd_alias_list_t* fireLastSound;
		snd_alias_list_t* fireLastSoundPlayer;
		snd_alias_list_t* emptyFireSound;
		snd_alias_list_t* emptyFireSoundPlayer;
		snd_alias_list_t* meleeSwipeSound;
		snd_alias_list_t* meleeSwipeSoundPlayer;
		snd_alias_list_t* meleeHitSound;
		snd_alias_list_t* meleeMissSound;
		snd_alias_list_t* rechamberSound;
		snd_alias_list_t* rechamberSoundPlayer;
		snd_alias_list_t* reloadSound;
		snd_alias_list_t* reloadSoundPlayer;
		snd_alias_list_t* reloadEmptySound;
		snd_alias_list_t* reloadEmptySoundPlayer;
		snd_alias_list_t* reloadStartSound;
		snd_alias_list_t* reloadStartSoundPlayer;
		snd_alias_list_t* reloadEndSound;
		snd_alias_list_t* reloadEndSoundPlayer;
		snd_alias_list_t* detonateSound;
		snd_alias_list_t* detonateSoundPlayer;
		snd_alias_list_t* nightVisionWearSound;
		snd_alias_list_t* nightVisionWearSoundPlayer;
		snd_alias_list_t* nightVisionRemoveSound;
		snd_alias_list_t* nightVisionRemoveSoundPlayer;
		snd_alias_list_t* altSwitchSound;
		snd_alias_list_t* altSwitchSoundPlayer;
		snd_alias_list_t* raiseSound;
		snd_alias_list_t* raiseSoundPlayer;
		snd_alias_list_t* firstRaiseSound;
		snd_alias_list_t* firstRaiseSoundPlayer;
		snd_alias_list_t* putawaySound;
		snd_alias_list_t* putawaySoundPlayer;
		snd_alias_list_t** bounceSound;
		FxEffectDef* viewShellEjectEffect;
		FxEffectDef* worldShellEjectEffect;
		FxEffectDef* viewLastShotEjectEffect;
		FxEffectDef* worldLastShotEjectEffect;
		Material* reticleCenter;
		Material* reticleSide;
		int iReticleCenterSize;
		int iReticleSideSize;
		int iReticleMinOfs;
		activeReticleType_t activeReticleType;
		float vStandMove[3];
		float vStandRot[3];
		float vDuckedOfs[3];
		float vDuckedMove[3];
		float vDuckedRot[3];
		float vProneOfs[3];
		float vProneMove[3];
		float vProneRot[3];
		float fPosMoveRate;
		float fPosProneMoveRate;
		float fStandMoveMinSpeed;
		float fDuckedMoveMinSpeed;
		float fProneMoveMinSpeed;
		float fPosRotRate;
		float fPosProneRotRate;
		float fStandRotMinSpeed;
		float fDuckedRotMinSpeed;
		float fProneRotMinSpeed;
		XModel* worldModel[16];
		XModel* worldClipModel;
		XModel* rocketModel;
		XModel* knifeModel;
		XModel* worldKnifeModel;
		Material* hudIcon;
		weaponIconRatioType_t hudIconRatio;
		Material* ammoCounterIcon;
		weaponIconRatioType_t ammoCounterIconRatio;
		ammoCounterClipType_t ammoCounterClip;
		int iStartAmmo;
		const char* szAmmoName;
		int iAmmoIndex;
		const char* szClipName;
		int iClipIndex;
		int iMaxAmmo;
		int iClipSize;
		int shotCount;
		const char* szSharedAmmoCapName;
		int iSharedAmmoCapIndex;
		int iSharedAmmoCap;
		int damage;
		int playerDamage;
		int iMeleeDamage;
		int iDamageType;
		int iFireDelay;
		int iMeleeDelay;
		int meleeChargeDelay;
		int iDetonateDelay;
		int iFireTime;
		int iRechamberTime;
		int iRechamberBoltTime;
		int iHoldFireTime;
		int iDetonateTime;
		int iMeleeTime;
		int meleeChargeTime;
		int iReloadTime;
		int reloadShowRocketTime;
		int iReloadEmptyTime;
		int iReloadAddTime;
		int iReloadStartTime;
		int iReloadStartAddTime;
		int iReloadEndTime;
		int iDropTime;
		int iRaiseTime;
		int iAltDropTime;
		int iAltRaiseTime;
		int quickDropTime;
		int quickRaiseTime;
		int iFirstRaiseTime;
		int iEmptyRaiseTime;
		int iEmptyDropTime;
		int sprintInTime;
		int sprintLoopTime;
		int sprintOutTime;
		int nightVisionWearTime;
		int nightVisionWearTimeFadeOutEnd;
		int nightVisionWearTimePowerUp;
		int nightVisionRemoveTime;
		int nightVisionRemoveTimePowerDown;
		int nightVisionRemoveTimeFadeInStart;
		int fuseTime;
		int aiFuseTime;
		int requireLockonToFire;
		int noAdsWhenMagEmpty;
		int avoidDropCleanup;
		float autoAimRange;
		float aimAssistRange;
		float aimAssistRangeAds;
		float aimPadding;
		float enemyCrosshairRange;
		int crosshairColorChange;
		float moveSpeedScale;
		float adsMoveSpeedScale;
		float sprintDurationScale;
		float fAdsZoomFov;
		float fAdsZoomInFrac;
		float fAdsZoomOutFrac;
		Material* overlayMaterial;
		Material* overlayMaterialLowRes;
		weapOverlayReticle_t overlayReticle;
		WeapOverlayInteface_t overlayInterface;
		float overlayWidth;
		float overlayHeight;
		float fAdsBobFactor;
		float fAdsViewBobMult;
		float fHipSpreadStandMin;
		float fHipSpreadDuckedMin;
		float fHipSpreadProneMin;
		float hipSpreadStandMax;
		float hipSpreadDuckedMax;
		float hipSpreadProneMax;
		float fHipSpreadDecayRate;
		float fHipSpreadFireAdd;
		float fHipSpreadTurnAdd;
		float fHipSpreadMoveAdd;
		float fHipSpreadDuckedDecay;
		float fHipSpreadProneDecay;
		float fHipReticleSidePos;
		int iAdsTransInTime;
		int iAdsTransOutTime;
		float fAdsIdleAmount;
		float fHipIdleAmount;
		float adsIdleSpeed;
		float hipIdleSpeed;
		float fIdleCrouchFactor;
		float fIdleProneFactor;
		float fGunMaxPitch;
		float fGunMaxYaw;
		float swayMaxAngle;
		float swayLerpSpeed;
		float swayPitchScale;
		float swayYawScale;
		float swayHorizScale;
		float swayVertScale;
		float swayShellShockScale;
		float adsSwayMaxAngle;
		float adsSwayLerpSpeed;
		float adsSwayPitchScale;
		float adsSwayYawScale;
		float adsSwayHorizScale;
		float adsSwayVertScale;
		int bRifleBullet;
		int armorPiercing;
		int bBoltAction;
		int aimDownSight;
		int bRechamberWhileAds;
		float adsViewErrorMin;
		float adsViewErrorMax;
		int bCookOffHold;
		int bClipOnly;
		int adsFireOnly;
		int cancelAutoHolsterWhenEmpty;
		int suppressAmmoReserveDisplay;
		int enhanced;
		int laserSightDuringNightvision;
		Material* killIcon;
		weaponIconRatioType_t killIconRatio;
		int flipKillIcon;
		Material* dpadIcon;
		weaponIconRatioType_t dpadIconRatio;
		int bNoPartialReload;
		int bSegmentedReload;
		int iReloadAmmoAdd;
		int iReloadStartAdd;
		const char* szAltWeaponName;
		unsigned int altWeaponIndex;
		int iDropAmmoMin;
		int iDropAmmoMax;
		int blocksProne;
		int silenced;
		int iExplosionRadius;
		int iExplosionRadiusMin;
		int iExplosionInnerDamage;
		int iExplosionOuterDamage;
		float damageConeAngle;
		int iProjectileSpeed;
		int iProjectileSpeedUp;
		int iProjectileSpeedForward;
		int iProjectileActivateDist;
		float projLifetime;
		float timeToAccelerate;
		float projectileCurvature;
		XModel* projectileModel;
		weapProjExposion_t projExplosion;
		FxEffectDef* projExplosionEffect;
		int projExplosionEffectForceNormalUp;
		FxEffectDef* projDudEffect;
		snd_alias_list_t* projExplosionSound;
		snd_alias_list_t* projDudSound;
		int bProjImpactExplode;
		WeapStickinessType stickiness;
		int hasDetonator;
		int timedDetonation;
		int rotate;
		int holdButtonToThrow;
		int freezeMovementWhenFiring;
		float lowAmmoWarningThreshold;
		float parallelBounce[29];
		float perpendicularBounce[29];
		FxEffectDef* projTrailEffect;
		float vProjectileColor[3];
		guidedMissileType_t guidedMissileType;
		float maxSteeringAccel;
		int projIgnitionDelay;
		FxEffectDef* projIgnitionEffect;
		snd_alias_list_t* projIgnitionSound;
		float fAdsAimPitch;
		float fAdsCrosshairInFrac;
		float fAdsCrosshairOutFrac;
		int adsGunKickReducedKickBullets;
		float adsGunKickReducedKickPercent;
		float fAdsGunKickPitchMin;
		float fAdsGunKickPitchMax;
		float fAdsGunKickYawMin;
		float fAdsGunKickYawMax;
		float fAdsGunKickAccel;
		float fAdsGunKickSpeedMax;
		float fAdsGunKickSpeedDecay;
		float fAdsGunKickStaticDecay;
		float fAdsViewKickPitchMin;
		float fAdsViewKickPitchMax;
		float fAdsViewKickYawMin;
		float fAdsViewKickYawMax;
		float fAdsViewKickCenterSpeed;
		float fAdsViewScatterMin;
		float fAdsViewScatterMax;
		float fAdsSpread;
		int hipGunKickReducedKickBullets;
		float hipGunKickReducedKickPercent;
		float fHipGunKickPitchMin;
		float fHipGunKickPitchMax;
		float fHipGunKickYawMin;
		float fHipGunKickYawMax;
		float fHipGunKickAccel;
		float fHipGunKickSpeedMax;
		float fHipGunKickSpeedDecay;
		float fHipGunKickStaticDecay;
		float fHipViewKickPitchMin;
		float fHipViewKickPitchMax;
		float fHipViewKickYawMin;
		float fHipViewKickYawMax;
		float fHipViewKickCenterSpeed;
		float fHipViewScatterMin;
		float fHipViewScatterMax;
		float fightDist;
		float maxDist;
		const char* accuracyGraphName[2];
		float(*accuracyGraphKnots[2])[2];
		float(*originalAccuracyGraphKnots[2])[2];
		int accuracyGraphKnotCount[2];
		int originalAccuracyGraphKnotCount[2];
		int iPositionReloadTransTime;
		float leftArc;
		float rightArc;
		float topArc;
		float bottomArc;
		float accuracy;
		float aiSpread;
		float playerSpread;
		float minTurnSpeed[2];
		float maxTurnSpeed[2];
		float pitchConvergenceTime;
		float yawConvergenceTime;
		float suppressTime;
		float maxRange;
		float fAnimHorRotateInc;
		float fPlayerPositionDist;
		const char* szUseHintString;
		const char* dropHintString;
		int iUseHintStringIndex;
		int dropHintStringIndex;
		float horizViewJitter;
		float vertViewJitter;
		const char* szScript;
		float fOOPosAnimLength[2];
		int minDamage;
		int minPlayerDamage;
		float fMaxDamageRange;
		float fMinDamageRange;
		float destabilizationRateTime;
		float destabilizationCurvatureMax;
		int destabilizeDistance;
		float locationDamageMultipliers[19];
		const char* fireRumble;
		const char* meleeImpactRumble;
		float adsDofStart;
		float adsDofEnd;
	};

	union XAssetHeader
	{
		void* data;
		XModelPieces* xmodelPieces;
		PhysPreset* physPreset;
		XAnimParts* parts;
		XModel* model;
		Material* material;
		// 			MaterialPixelShader *pixelShader;
		// 			MaterialVertexShader *vertexShader;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		snd_alias_list_t* sound;
		// 			SndCurve *sndCurve;
		LoadedSound* loadSnd;
		clipMap_t* clipMap;
		ComWorld* comWorld;
		// 			GameWorldSp *gameWorldSp;
		GameWorldMp* gameWorldMp;
		MapEnts* mapEnts;
		GfxWorld* gfxWorld;
		GfxLightDef* lightDef;
		// 			Font_s *font;
		// 			MenuList *menuList;
		// 			menuDef_t *menu;
		LocalizeEntry* localize;
		WeaponDef* weapon;
		// 			SndDriverGlobals *sndDriverGlobals;
		FxEffectDef* fx;
		// 			FxImpactTable *impactFx;
		RawFile* rawfile;
		// 			StringTable *stringTable;
	};

	struct XAsset
	{
		XAssetType type;
		XAssetHeader header;
	};

	struct XAssetEntry
	{
		XAsset asset;
		char zoneIndex;
		char inuse;
		unsigned __int16 nextHash;
		unsigned __int16 nextOverride;
		unsigned __int16 usageFrame;
	};

	union XAssetEntryPoolEntry
	{
		XAssetEntry entry;
		XAssetEntryPoolEntry* next;
	};

	struct XBlock
	{
		char* data;
		unsigned int size;
	};


	struct XZoneMemory
	{
		XBlock blocks[9];
		char* lockedVertexData;
		char* lockedIndexData;
		void* vertexBuffer;
		void* indexBuffer;
	};

	struct XZone
	{
		char name[64];
		int flags;
		int allocType;
		XZoneMemory mem;
		int fileSize;
		char modZone;
	};

#ifdef __cplusplus
}
#endif

#pragma endregion

#ifdef __cplusplus
}
#endif
