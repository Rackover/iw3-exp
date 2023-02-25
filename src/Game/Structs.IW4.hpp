#pragma once

#include "Structs.hpp"

#ifdef __cplusplus
namespace Game::IW4
{
	struct Material;

	enum XAssetType
	{
		ASSET_TYPE_PHYSPRESET = 0x0,
		ASSET_TYPE_PHYSCOLLMAP = 0x1,
		ASSET_TYPE_XANIMPARTS = 0x2,
		ASSET_TYPE_XMODEL_SURFS = 0x3,
		ASSET_TYPE_XMODEL = 0x4,
		ASSET_TYPE_MATERIAL = 0x5,
		ASSET_TYPE_PIXELSHADER = 0x6,
		ASSET_TYPE_VERTEXSHADER = 0x7,
		ASSET_TYPE_VERTEXDECL = 0x8,
		ASSET_TYPE_TECHNIQUE_SET = 0x9,
		ASSET_TYPE_IMAGE = 0xA,
		ASSET_TYPE_SOUND = 0xB,
		ASSET_TYPE_SOUND_CURVE = 0xC,
		ASSET_TYPE_LOADED_SOUND = 0xD,
		ASSET_TYPE_CLIPMAP_SP = 0xE,
		ASSET_TYPE_CLIPMAP_MP = 0xF,
		ASSET_TYPE_COMWORLD = 0x10,
		ASSET_TYPE_GAMEWORLD_SP = 0x11,
		ASSET_TYPE_GAMEWORLD_MP = 0x12,
		ASSET_TYPE_MAP_ENTS = 0x13,
		ASSET_TYPE_FXWORLD = 0x14,
		ASSET_TYPE_GFXWORLD = 0x15,
		ASSET_TYPE_LIGHT_DEF = 0x16,
		ASSET_TYPE_UI_MAP = 0x17,
		ASSET_TYPE_FONT = 0x18,
		ASSET_TYPE_MENULIST = 0x19,
		ASSET_TYPE_MENU = 0x1A,
		ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
		ASSET_TYPE_WEAPON = 0x1C,
		ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
		ASSET_TYPE_FX = 0x1E,
		ASSET_TYPE_IMPACT_FX = 0x1F,
		ASSET_TYPE_AITYPE = 0x20,
		ASSET_TYPE_MPTYPE = 0x21,
		ASSET_TYPE_CHARACTER = 0x22,
		ASSET_TYPE_XMODELALIAS = 0x23,
		ASSET_TYPE_RAWFILE = 0x24,
		ASSET_TYPE_STRINGTABLE = 0x25,
		ASSET_TYPE_LEADERBOARD = 0x26,
		ASSET_TYPE_STRUCTURED_DATA_DEF = 0x27,
		ASSET_TYPE_TRACER = 0x28,
		ASSET_TYPE_VEHICLE = 0x29,
		ASSET_TYPE_ADDON_MAP_ENTS = 0x2A,
		ASSET_TYPE_COUNT = 0x2B,
		ASSET_TYPE_STRING = 0x2B,
		ASSET_TYPE_ASSETLIST = 0x2C,
		ASSET_TYPE_INVALID = -1,
	};

	enum StaticModelFlag : char
	{
		STATIC_MODEL_FLAG_SUB_INDEX_MASK = 0x7,
		STATIC_MODEL_FLAG_NO_CAST_SHADOW = 0x10,
		STATIC_MODEL_FLAG_GROUND_LIGHTING = 0x20,
	};

	enum SndChannel
	{
		SND_CHANNEL_PHYSICS,
		SND_CHANNEL_AMBDIST1,
		SND_CHANNEL_AMBDIST2,
		SND_CHANNEL_AUTO,
		SND_CHANNEL_AUTO2,
		SND_CHANNEL_AUTODOG,
		SND_CHANNEL_BULLETIMPACT,
		SND_CHANNEL_BULLETWHIZBY,
		SND_CHANNEL_EXPLOSIVEIMPACT,
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
		SND_CHANNEL_LOCAL3,
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

	enum FogTypes
	{
		FOG_NORMAL = 0x1,
		FOG_DFOG = 0x2,
	};

	enum GfxRenderTargetId
	{
		R_RENDERTARGET_SAVED_SCREEN = 0x0,
		R_RENDERTARGET_FRAME_BUFFER = 0x1,
		R_RENDERTARGET_SCENE = 0x2,
		R_RENDERTARGET_RESOLVED_POST_SUN = 0x3,
		R_RENDERTARGET_RESOLVED_SCENE = 0x4,
		R_RENDERTARGET_FLOAT_Z = 0x5,
		R_RENDERTARGET_PINGPONG_0 = 0x6,
		R_RENDERTARGET_PINGPONG_1 = 0x7,
		R_RENDERTARGET_POST_EFFECT_0 = 0x8,
		R_RENDERTARGET_POST_EFFECT_1 = 0x9,
		R_RENDERTARGET_SHADOWMAP_LARGE = 0xA,
		R_RENDERTARGET_SHADOWMAP_SMALL = 0xB,
		R_RENDERTARGET_COUNT = 0xC,
		R_RENDERTARGET_NONE = 0xD,
	};


	struct GfxImageLoadDef
	{
		char levelCount;
		char pad[3];
		int flags;
		int format;
		int resourceSize;
		char data[1];
	};

	union GfxTexture
	{
		GfxImageLoadDef* loadDef;
	};

	struct GfxImage
	{
		GfxTexture texture;
		unsigned char mapType;
		unsigned char semantic;
		unsigned char category;
		bool useSrgbReads;
		Game::IW3::Picmip picmip;
		bool noPicmip;
		char track;
		Game::IW3::CardMemory cardMemory;
		unsigned short width;
		unsigned short height;
		unsigned short depth;
		bool delayLoadPixels;
		const char* name;
	};

	struct GfxImageFileHeader
	{
		char tag[3];
		char version;
		int flags;
		char format;
		short dimensions[3];
		int fileSizeForPicmip[4];
	};

	struct Bounds
	{
		vec3_t midPoint; // mins
		vec3_t halfSize; // maxs

		void compute(vec3_t mins, vec3_t maxs);
		void max(vec3_t& out);
		void min(vec3_t& out);
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

	enum XAnimPartType
	{
		PART_TYPE_NO_QUAT = 0x0,
		PART_TYPE_HALF_QUAT = 0x1,
		PART_TYPE_FULL_QUAT = 0x2,
		PART_TYPE_HALF_QUAT_NO_SIZE = 0x3,
		PART_TYPE_FULL_QUAT_NO_SIZE = 0x4,
		PART_TYPE_SMALL_TRANS = 0x5,
		PART_TYPE_TRANS = 0x6,
		PART_TYPE_TRANS_NO_SIZE = 0x7,
		PART_TYPE_NO_TRANS = 0x8,
		PART_TYPE_ALL = 0x9,
	};

	union XAnimIndices
	{
		char* _1;
		unsigned short* _2;
		void* data;
	};

	struct XAnimNotifyInfo
	{
		unsigned __int16 name;
		float time;
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
		XAnimPartTransData u;
	};

	struct XAnimDeltaPartQuatDataFrames2
	{
		__int16* frames;
		char indices[1];
	};

	union XAnimDeltaPartQuatData2
	{
		XAnimDeltaPartQuatDataFrames2 frames;
		__int16 frame0[2];
	};

	struct XAnimDeltaPartQuat2
	{
		unsigned __int16 size;
		XAnimDeltaPartQuatData2 u;
	};

	struct XAnimDeltaPartQuatDataFrames
	{
		__int16* frames;
		char indices[1];
	};

	union XAnimDeltaPartQuatData
	{
		XAnimDeltaPartQuatDataFrames frames;
		__int16 frame0[4];
	};

	struct XAnimDeltaPartQuat
	{
		unsigned __int16 size;
		XAnimDeltaPartQuatData u;
	};

	struct XAnimDeltaPart
	{
		XAnimPartTrans* trans;
		XAnimDeltaPartQuat2* quat2;
		XAnimDeltaPartQuat* quat;
	};

	struct XAnimParts
	{
		const char* name; // 0
		unsigned short dataByteCount; // 4
		unsigned short dataShortCount; // 6
		unsigned short dataIntCount; // 8
		unsigned short randomDataByteCount; // 10 - 0xA
		unsigned short randomDataIntCount;// 12 - 0xC
		unsigned short framecount; // 14 - 0xE
		char bLoop; // 16
		char boneCount[10]; // 17
		char notetrackCount; // 27
		char pad1; // 28
		char bDelta; // 29
		char assetType; // 30
		char pad2; // 31
		int randomDataShortCount; // 32 - 0x20
		int indexcount; // 36 - 0x24
		float framerate; // 40 - 0x28
		float frequency; // 44 - 0x2C
		short* tagnames; // 48 - 0x30
		char* dataByte;// 52 - 0x34
		short* dataShort; // 56 - 0x38
		int* dataInt; // 60 - 0x3C
		short* randomDataShort; // 64 - 0x40
		char* randomDataByte; // 68 - 0x44
		int* randomDataInt; // 72 - 0x48
		XAnimIndices indices; // 76 - 0x4C
		XAnimNotifyInfo* notetracks; // 80 - 0x50
		XAnimDeltaPart* delta; // 84 - 0x54
								// 88 - 0x58
	};

	struct XModelTagPos
	{
		float x;
		float y;
		float z;
	};

	struct XBoneInfo
	{
		Bounds bounds;
		float radiusSquared;
	};

	struct GfxSurface
	{
		IW3::srfTriangles_t tris;
		Material* material;
		unsigned char lightmapIndex;
		unsigned char reflectionProbeIndex;
		unsigned char primaryLightIndex;
		unsigned char flags;
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
	};

	struct GfxPortal
	{
		GfxPortalWritable writable;
		DpvsPlane plane;
		float(*vertices)[3];
		unsigned __int16 cellIndex;
		char vertexCount;
		float hullAxis[2][3];
	};

	struct GfxCell
	{
		Bounds bounds;
		int portalCount;
		GfxPortal* portals;
		unsigned char reflectionProbeCount;
		unsigned char* reflectionProbes;
	};

#pragma pack(push, 4)
	struct GfxPackedPlacement
	{
		float origin[3];
		vec3_t axis[3];
		float scale;
	};

	struct GfxStaticModelDrawInst
	{
		GfxPackedPlacement placement;
		struct XModel* model;
		unsigned __int16 cullDist;
		unsigned __int16 lightingHandle;
		unsigned char reflectionProbeIndex;
		unsigned char primaryLightIndex;
		unsigned char flags;
		unsigned char firstMtlSkinIndex;
		IW3::GfxColor groundLighting;
		unsigned __int16 cacheId[4];
	};

	struct GfxBrushModelWritable
	{
		Bounds bounds;
	};

	struct GfxBrushModel
	{
		GfxBrushModelWritable writable;
		Bounds bounds;
		float radius;
		unsigned short surfaceCount;
		unsigned short startSurfIndex;
		unsigned short surfaceCountNoDecal;
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
		bool diffuseColorHasBeenSet;
		float angles[3];
	};

	struct GfxReflectionProbe
	{
		float origin[3];
	};

	struct GfxLightmapArray
	{
		GfxImage* primary;
		GfxImage* secondary;
	};

	struct GfxWorldDraw
	{
		unsigned int reflectionProbeCount;
		GfxImage** reflectionImages;
		GfxReflectionProbe* reflectionProbes;
		GfxTexture* reflectionProbeTextures; //Count = refelctionProbeCount
		int lightmapCount;
		GfxLightmapArray* lightmaps;
		GfxTexture* lightmapPrimaryTextures; //Count = lightmapCount
		GfxTexture* lightmapSecondaryTextures; //Count = lightmapCount
		GfxImage* skyImage;
		GfxImage* outdoorImage;
		unsigned int vertexCount;
		IW3::GfxWorldVertexData vd;
		unsigned int vertexLayerDataSize;
		IW3::GfxWorldVertexLayerData vld;
		int indexCount;
		unsigned __int16* indices;
	};

	struct GfxSky
	{
		int skySurfCount;
		int* skyStartSurfs;
		GfxImage* skyImage;
		int skySamplerState;
	};

	struct GfxHeroOnlyLight
	{
		char type;
		char unused[3];
		float color[3];
		float dir[3];
		float up[3];
		//float origin[3];
		float radius;
		float cosHalfFovOuter;
		float cosHalfFovInner;
		int exponent;
	};

	struct GfxAabbTree
	{
		Bounds bounds;
		unsigned __int16 childCount;
		unsigned __int16 surfaceCount;
		unsigned __int16 startSurfIndex;
		unsigned __int16 surfaceCountNoDecal;
		unsigned __int16 startSurfIndexNoDecal;
		unsigned __int16 smodelIndexCount;
		unsigned __int16* smodelIndexes;
		int childrenOffset;
	};

	struct GfxCellTree
	{
		GfxAabbTree* aabbTree;
	};

	struct GfxCellTreeCount
	{
		int aabbTreeCount;
	};

	struct GfxStaticModelInst
	{
		Bounds bounds;
		float lightingOrigin[3];
	};

	struct GfxSurfaceBounds
	{
		Bounds bounds;
		//char flags;
	};


	struct GfxDrawSurfFields
	{
		unsigned __int64 objectId : 16;
		unsigned __int64 reflectionProbeIndex : 8;
		unsigned __int64 hasGfxEntIndex : 1;
		unsigned __int64 customIndex : 5;
		unsigned __int64 materialSortedIndex : 12;
		unsigned __int64 prepass : 2;
		unsigned __int64 useHeroLighting : 1;
		unsigned __int64 sceneLightIndex : 8;
		unsigned __int64 surfType : 4;
		unsigned __int64 primarySortKey : 6;
		unsigned __int64 unused : 1;
	};

	union GfxDrawSurf
	{
		GfxDrawSurfFields fields;
		unsigned long long packed;
	};

	struct GfxWorldDpvsStatic
	{
		unsigned int smodelCount;
		unsigned int staticSurfaceCount;
		unsigned int staticSurfaceCountNoDecal;
		unsigned int litOpaqueSurfsBegin;
		unsigned int litOpaqueSurfsEnd;
		unsigned int litTransSurfsBegin;
		unsigned int litTransSurfsEnd;
		unsigned int shadowCasterSurfsBegin;
		unsigned int shadowCasterSurfsEnd;
		unsigned int emissiveSurfsBegin;
		unsigned int emissiveSurfsEnd;
		unsigned int smodelVisDataCount;
		unsigned int surfaceVisDataCount;
		char* smodelVisData[3];
		char* surfaceVisData[3];
		unsigned __int16* sortedSurfIndex;
		GfxStaticModelInst* smodelInsts;
		GfxSurface* surfaces;
		GfxSurfaceBounds* surfacesBounds;
		GfxStaticModelDrawInst* smodelDrawInsts;
		GfxDrawSurf* surfaceMaterials;
		unsigned int* surfaceCastsSunShadow;
		volatile int usageCount;
	};

	struct MaterialMemory
	{
		Material* material;
		int memory;
	};

	struct GfxWorld
	{
		const char* name;
		const char* baseName;
		int planeCount;
		int nodeCount;
		unsigned int surfaceCount;
		int skyCount;
		GfxSky* skies;
		unsigned int lastSunPrimaryLightIndex;
		unsigned int primaryLightCount;
		unsigned int sortKeyLitDecal;
		unsigned int sortKeyEffectDecal;
		unsigned int sortKeyEffectAuto;
		unsigned int sortKeyDistortion;
		IW3::GfxWorldDpvsPlanes dpvsPlanes;
		GfxCellTreeCount* aabbTreeCounts;
		GfxCellTree* aabbTrees;
		GfxCell* cells;
		GfxWorldDraw draw;
		IW3::GfxLightGrid lightGrid;
		int modelCount;
		GfxBrushModel* models;
		Bounds bounds;
		unsigned int checksum;
		int materialMemoryCount;
		MaterialMemory* materialMemory;
		IW3::sunflare_t sun;
		float outdoorLookupMatrix[4][4];
		GfxImage* outdoorImage;
		unsigned int* cellCasterBits;
		unsigned int* cellHasSunLitSurfsBits;
		IW3::GfxSceneDynModel* sceneDynModel;
		IW3::GfxSceneDynBrush* sceneDynBrush;
		unsigned int* primaryLightEntityShadowVis;
		unsigned int* primaryLightDynEntShadowVis[2];
		char* nonSunPrimaryLightForModelDynEnt;
		IW3::GfxShadowGeometry* shadowGeom;
		IW3::GfxLightRegion* lightRegion;
		GfxWorldDpvsStatic dpvs;
		IW3::GfxWorldDpvsDynamic dpvsDyn;
		unsigned int mapVtxChecksum;
		unsigned int heroOnlyLightCount;
		GfxHeroOnlyLight* heroOnlyLights;
		char fogTypesAllowed;
	};
#pragma pack(pop)

	enum file_image_flags_t
	{
		IMG_FLAG_NOPICMIP = 0x1,
		IMG_FLAG_NOMIPMAPS = 0x2,
		IMG_FLAG_STREAMING = 0x4,
		IMG_FLAG_LEGACY_NORMALS = 0x8,
		IMG_FLAG_CLAMP_U = 0x10,
		IMG_FLAG_CLAMP_V = 0x20,
		IMG_FLAG_ALPHA_WEIGHTED_COLORS = 0x40,
		IMG_FLAG_DXTC_APPROX_WEIGHTS = 0x80,
		IMG_FLAG_GAMMA_NONE = 0x0,
		IMG_FLAG_GAMMA_SRGB = 0x100,
		IMG_FLAG_GAMMA_PWL = 0x200,
		IMG_FLAG_GAMMA_2 = 0x300,
		IMG_FLAG_MAPTYPE_2D = 0x0,
		IMG_FLAG_MAPTYPE_CUBE = 0x10000,
		IMG_FLAG_MAPTYPE_3D = 0x20000,
		IMG_FLAG_MAPTYPE_1D = 0x30000,
		IMG_FLAG_NORMALMAP = 0x40000,
		IMG_FLAG_INTENSITY_TO_ALPHA = 0x80000,
		IMG_FLAG_DYNAMIC = 0x1000000,
		IMG_FLAG_RENDER_TARGET = 0x2000000,
		IMG_FLAG_SYSTEMMEM = 0x4000000,
	};

	struct G_GlassPiece
	{
		unsigned __int16 damageTaken;
		unsigned __int16 collapseTime;
		int lastStateChangeTime;
		char impactDir;
		char impactPos[2];
	};

	struct G_GlassName
	{
		char* nameStr;
		unsigned __int16 name;
		unsigned __int16 pieceCount;
		unsigned __int16* pieceIndices;
	};

	struct G_GlassData
	{
		G_GlassPiece* glassPieces;
		unsigned int pieceCount;
		unsigned __int16 damageToWeaken;
		unsigned __int16 damageToDestroy;
		unsigned int glassNameCount;
		G_GlassName* glassNames;
		char pad[108];
	};


	struct GameWorldMp
	{
		const char* name;
		G_GlassData* g_glassData;
	};

	struct CollisionAabbTree
	{
		float midPoint[3];
		unsigned __int16 materialIndex;
		unsigned __int16 childCount;
		float halfSize[3];
		IW3::CollisionAabbTreeIndex u;
	};

	struct cbrushside_t
	{
		IW3::cplane_s* plane;
		unsigned __int16 materialNum;
		char firstAdjacentSideOffset;
		char edgeCount;
	};

	struct cbrush_t
	{
		unsigned __int16 numsides;
		unsigned __int16 glassPieceIndex;
		cbrushside_t* sides;
		char* baseAdjacentSide;
		__int16 axialMaterialNum[2][3];
		char firstAdjacentSideOffsets[2][3];
		char edgeCount[2][3];
	};

#pragma pack(push, 4)
	struct BrushWrapper
	{
		Bounds bounds;
		cbrush_t brush;
		int totalEdgeCount;
		IW3::cplane_s* planes;
	};
#pragma pack(pop)

	struct PhysGeomInfo
	{
		BrushWrapper* brush;
		int type;
		float orientation[3][3];
		float offset[3];
		float halfLengths[3];
	};

	struct PhysCollmap
	{
		const char* name;
		unsigned int count;
		PhysGeomInfo* geoms;
		IW3::PhysMass mass;
		Bounds bounds;
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
		IW3::XSurfaceVertexInfo vertInfo;
		IW3::GfxPackedVertex* verts0;
		unsigned int vertListCount;
		IW3::XRigidVertList* vertList;
		int partBits[6];
	};

	struct XModelSurfs
	{
		const char* name;
		XSurface* surfaces;
		int numSurfaces;
		int partBits[6];
	};

	struct XModelLodInfo
	{
		float dist;
		unsigned __int16 numsurfs;
		unsigned __int16 surfIndex;
		XModelSurfs* modelSurfs;
		int partBits[6];
		XSurface* surfs;
		char lod;
		char smcBaseIndexPlusOne;
		char smcSubIndexMask;
		char smcBucket;
	};

	struct XModelCollSurf_s
	{
		IW3::XModelCollTri_s* collTris;
		int numCollTris;
		Bounds bounds;
		int boneIdx;
		int contents;
		int surfFlags;
	};

	struct XModel
	{
		const char* name;
		unsigned char numBones;
		unsigned char numRootBones;
		unsigned char numsurfs;
		char lodRampType;
		float scale;
		unsigned int noScalePartBits[6];
		unsigned __int16* boneNames;
		unsigned char* parentList;
		__int16* quats;
		float* trans;
		unsigned char* partClassification;
		IW3::DObjAnimMat* baseMat;
		Material** materialHandles;
		XModelLodInfo lodInfo[4];
		char maxLoadedLod;
		char numLods;
		char collLod;
		unsigned char flags;
		XModelCollSurf_s* collSurfs;
		int numCollSurfs;
		int contents;
		XBoneInfo* boneInfo;
		float radius;
		Bounds bounds;
		int memUsage;
		bool bad;
		IW3::PhysPreset* physPreset;
		PhysCollmap* physCollmap;
	};

	struct cStaticModel_t
	{
		XModel* xmodel;
		float origin[3];
		float invScaledAxis[3][3];
		float absmin[3];
		float absmax[3];
	};

	struct dmaterial_t
	{
		char* material;
		int surfaceFlags;
		int contentFlags;
	};

	struct cLeaf_t
	{
		unsigned __int16 firstCollAabbIndex;
		unsigned __int16 collAabbCount;
		int brushContents;
		int terrainContents;
		Bounds bounds;
		int leafBrushNode;
	};

	struct cLeafBrushNode_t
	{
		char axis;
		__int16 leafBrushCount;
		int contents;
		IW3::cLeafBrushNodeLeaf_t data;
		char pad[8];
	};

	struct cmodel_t
	{
		Bounds bounds;
		float radius;
		cLeaf_t leaf;
	};

	struct SModelAabbNode
	{
		Bounds bounds;
		unsigned __int16 firstChild;
		unsigned __int16 childCount;
	};

	enum FxElemType : char
	{
		FX_ELEM_TYPE_SPRITE_BILLBOARD = 0x0,
		FX_ELEM_TYPE_SPRITE_ORIENTED = 0x1,
		FX_ELEM_TYPE_TAIL = 0x2,
		FX_ELEM_TYPE_TRAIL = 0x3,
		FX_ELEM_TYPE_CLOUD = 0x4,
		FX_ELEM_TYPE_SPARKCLOUD = 0x5,
		FX_ELEM_TYPE_SPARKFOUNTAIN = 0x6,
		FX_ELEM_TYPE_MODEL = 0x7,
		FX_ELEM_TYPE_OMNI_LIGHT = 0x8,
		FX_ELEM_TYPE_SPOT_LIGHT = 0x9,
		FX_ELEM_TYPE_SOUND = 0xA,
		FX_ELEM_TYPE_DECAL = 0xB,
		FX_ELEM_TYPE_RUNNER = 0xC,
		FX_ELEM_TYPE_COUNT = 0xD,
		FX_ELEM_TYPE_LAST_SPRITE = 0x3,
		FX_ELEM_TYPE_LAST_DRAWN = 0x9,
	};


	union FxEffectDefRef
	{
		struct FxEffectDef* handle;
		const char* name;
	};

	union FxElemVisuals
	{
		const void* anonymous;
		struct Material* material;
		XModel* model;
		FxEffectDefRef effectDef;
		const char* soundName;
	};

	struct FxElemMarkVisuals
	{
		struct Material* materials[2];
	};

	union FxElemDefVisuals
	{
		FxElemMarkVisuals* markArray;
		FxElemVisuals* array;
		FxElemVisuals instance;
	};

	struct FxElemDef
	{
		int flags;
		IW3::FxSpawnDef spawn;
		IW3::FxFloatRange spawnRange;
		IW3::FxFloatRange fadeInRange;
		IW3::FxFloatRange fadeOutRange;
		float spawnFrustumCullRadius;
		IW3::FxIntRange spawnDelayMsec;
		IW3::FxIntRange lifeSpanMsec;
		IW3::FxFloatRange spawnOrigin[3];
		IW3::FxFloatRange spawnOffsetRadius;
		IW3::FxFloatRange spawnOffsetHeight;
		IW3::FxFloatRange spawnAngles[3];
		IW3::FxFloatRange angularVelocity[3];
		IW3::FxFloatRange initialRotation;
		IW3::FxFloatRange gravity;
		IW3::FxFloatRange reflectionFactor;
		IW3::FxElemAtlas atlas;
		FxElemType elemType;
		char visualCount;
		char velIntervalCount;
		char visStateIntervalCount;
		IW3::FxElemVelStateSample* velSamples;
		IW3::FxElemVisStateSample* visSamples;
		FxElemDefVisuals visuals;
		Bounds bounds;
		FxEffectDefRef effectOnImpact; // ~
		FxEffectDefRef effectOnDeath; // ~
		FxEffectDefRef effectEmitted; // ~
		IW3::FxFloatRange emitDist;
		IW3::FxFloatRange emitDistVariance;
		struct FxTrailDef* trailDef; // FxElemExtendedDef; IW3 only has trails
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
		int type; // Identical in iw3
		Game::IW3::GfxPlacement pose;
		XModel* xModel;
		unsigned short brushModel;
		unsigned short physicsBrushModel;
		FxEffectDef* destroyFx;
		struct PhysPreset* physPreset;
		int health;
		Game::IW3::PhysMass mass;
		int contents;
	};

	struct clipMap_t
	{
		const char* name;
		int isInUse;
		int planeCount;
		IW3::cplane_s* planes;
		unsigned int numStaticModels;
		cStaticModel_t* staticModelList;
		unsigned int numMaterials;
		dmaterial_t* materials;
		unsigned int numBrushSides;
		cbrushside_t* brushsides;
		unsigned int numBrushEdges;
		char* brushEdges;
		unsigned int numNodes;
		IW3::cNode_t* nodes;
		unsigned int numLeafs;
		cLeaf_t* leafs;
		unsigned int leafbrushNodesCount;
		IW3::cLeafBrushNode_s* leafbrushNodes;
		unsigned int numLeafBrushes;
		unsigned short* leafbrushes;
		unsigned int numLeafSurfaces;
		unsigned int* leafsurfaces;
		unsigned int vertCount;
		vec3_t* verts;
		int triCount;
		unsigned short* triIndices;
		char* triEdgeIsWalkable;
		int borderCount;
		IW3::CollisionBorder* borders;
		int partitionCount;
		IW3::CollisionPartition* partitions;
		int aabbTreeCount;
		CollisionAabbTree* aabbTrees;
		unsigned int numSubModels;
		cmodel_t* cmodels;
		unsigned short numBrushes;
		cbrush_t* brushes;
		Bounds* brushBounds;
		int* brushContents;
		struct MapEnts* mapEnts;
		unsigned short smodelNodeCount;
		SModelAabbNode* smodelNodes;
		unsigned short dynEntCount[2];
		DynEntityDef* dynEntDefList[2];
		void* dynEntPoseList[2];
		void* dynEntClientList[2];
		void* dynEntCollList[2];
		unsigned int checksum;
		char padding[48];
	};

	enum MaterialTechniqueType
	{
		TECHNIQUE_DEPTH_PREPASS = 0x0,
		TECHNIQUE_BUILD_FLOAT_Z = 0x1,
		TECHNIQUE_BUILD_SHADOWMAP_DEPTH = 0x2,
		TECHNIQUE_BUILD_SHADOWMAP_COLOR = 0x3,
		TECHNIQUE_UNLIT = 0x4,
		TECHNIQUE_EMISSIVE = 0x5,
		TECHNIQUE_EMISSIVE_DFOG = 0x6,
		TECHNIQUE_EMISSIVE_SHADOW = 0x7,
		TECHNIQUE_EMISSIVE_SHADOW_DFOG = 0x8,
		TECHNIQUE_LIT_BEGIN = 0x9,
		TECHNIQUE_LIT = 0x9,
		TECHNIQUE_LIT_DFOG = 0xA,
		TECHNIQUE_LIT_SUN = 0xB,
		TECHNIQUE_LIT_SUN_DFOG = 0xC,
		TECHNIQUE_LIT_SUN_SHADOW = 0xD,
		TECHNIQUE_LIT_SUN_SHADOW_DFOG = 0xE,
		TECHNIQUE_LIT_SPOT = 0xF,
		TECHNIQUE_LIT_SPOT_DFOG = 0x10,
		TECHNIQUE_LIT_SPOT_SHADOW = 0x11,
		TECHNIQUE_LIT_SPOT_SHADOW_DFOG = 0x12,
		TECHNIQUE_LIT_OMNI = 0x13,
		TECHNIQUE_LIT_OMNI_DFOG = 0x14,
		TECHNIQUE_LIT_OMNI_SHADOW = 0x15,
		TECHNIQUE_LIT_OMNI_SHADOW_DFOG = 0x16,
		TECHNIQUE_LIT_INSTANCED = 0x17,
		TECHNIQUE_LIT_INSTANCED_DFOG = 0x18,
		TECHNIQUE_LIT_INSTANCED_SUN = 0x19,
		TECHNIQUE_LIT_INSTANCED_SUN_DFOG = 0x1A,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW = 0x1B,
		TECHNIQUE_LIT_INSTANCED_SUN_SHADOW_DFOG = 0x1C,
		TECHNIQUE_LIT_INSTANCED_SPOT = 0x1D,
		TECHNIQUE_LIT_INSTANCED_SPOT_DFOG = 0x1E,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW = 0x1F,
		TECHNIQUE_LIT_INSTANCED_SPOT_SHADOW_DFOG = 0x20,
		TECHNIQUE_LIT_INSTANCED_OMNI = 0x21,
		TECHNIQUE_LIT_INSTANCED_OMNI_DFOG = 0x22,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW = 0x23,
		TECHNIQUE_LIT_INSTANCED_OMNI_SHADOW_DFOG = 0x24,
		TECHNIQUE_LIT_END = 0x25,
		TECHNIQUE_LIGHT_SPOT = 0x25,
		TECHNIQUE_LIGHT_OMNI = 0x26,
		TECHNIQUE_LIGHT_SPOT_SHADOW = 0x27,
		TECHNIQUE_FAKELIGHT_NORMAL = 0x28,
		TECHNIQUE_FAKELIGHT_VIEW = 0x29,
		TECHNIQUE_SUNLIGHT_PREVIEW = 0x2A,
		TECHNIQUE_CASE_TEXTURE = 0x2B,
		TECHNIQUE_WIREFRAME_SOLID = 0x2C,
		TECHNIQUE_WIREFRAME_SHADED = 0x2D,
		TECHNIQUE_DEBUG_BUMPMAP = 0x2E,
		TECHNIQUE_DEBUG_BUMPMAP_INSTANCED = 0x2F,
		TECHNIQUE_COUNT = 0x30,
		TECHNIQUE_TOTAL_COUNT = 0x31,
		TECHNIQUE_NONE = 0x32,
	};

	enum MaterialTextureSource : unsigned int
	{
		TEXTURE_SRC_CODE_BLACK = 0x0,
		TEXTURE_SRC_CODE_WHITE = 0x1,
		TEXTURE_SRC_CODE_IDENTITY_NORMAL_MAP = 0x2,
		TEXTURE_SRC_CODE_MODEL_LIGHTING = 0x3,
		TEXTURE_SRC_CODE_LIGHTMAP_PRIMARY = 0x4,
		TEXTURE_SRC_CODE_LIGHTMAP_SECONDARY = 0x5,
		TEXTURE_SRC_CODE_SHADOWMAP_SUN = 0x6,
		TEXTURE_SRC_CODE_SHADOWMAP_SPOT = 0x7,
		TEXTURE_SRC_CODE_FEEDBACK = 0x8,
		TEXTURE_SRC_CODE_RESOLVED_POST_SUN = 0x9,
		TEXTURE_SRC_CODE_RESOLVED_SCENE = 0xA,
		TEXTURE_SRC_CODE_POST_EFFECT_0 = 0xB,
		TEXTURE_SRC_CODE_POST_EFFECT_1 = 0xC,
		TEXTURE_SRC_CODE_LIGHT_ATTENUATION = 0xD,
		TEXTURE_SRC_CODE_OUTDOOR = 0xE,
		TEXTURE_SRC_CODE_FLOATZ = 0xF,
		TEXTURE_SRC_CODE_PROCESSED_FLOATZ = 0x10,
		TEXTURE_SRC_CODE_RAW_FLOATZ = 0x11,
		TEXTURE_SRC_CODE_HALF_PARTICLES = 0x12,
		TEXTURE_SRC_CODE_HALF_PARTICLES_Z = 0x13,
		TEXTURE_SRC_CODE_CASE_TEXTURE = 0x14,
		TEXTURE_SRC_CODE_CINEMATIC_Y = 0x15,
		TEXTURE_SRC_CODE_CINEMATIC_CR = 0x16,
		TEXTURE_SRC_CODE_CINEMATIC_CB = 0x17,
		TEXTURE_SRC_CODE_CINEMATIC_A = 0x18,
		TEXTURE_SRC_CODE_REFLECTION_PROBE = 0x19,
		TEXTURE_SRC_CODE_ALTERNATE_SCENE = 0x1A,
		TEXTURE_SRC_CODE_COUNT = 0x1B,
	};

	enum StateFlags : unsigned char {
		STATE_FLAG_CULL_BACK = 0x1,
		STATE_FLAG_AMBIENT = 0x2,
		STATE_FLAG_DECAL = 0x4,
		STATE_FLAG_WRITES_DEPTH = 0x8,
		STATE_FLAG_USES_DEPTH_BUFFER = 0x10,
		STATE_FLAG_USES_STENCIL_BUFFER = 0x20,
		STATE_FLAG_CULL_BACK_SHADOW = 0x40,
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
		MTL_STATE_SET_GAMMA_WRITE = 0x7,
		MTL_STATE_SET_POLYGON_OFFSET = 0x8,
		MTL_STATE_SET_STENCIL = 0x9,
		MTL_STATE_SET_WIREFRAME = 0xA,
		MTL_STATE_SET_COUNT = 0xB,
	};

	struct MaterialGameFlagsFields
	{
		unsigned char unk1 : 1; // 0x1
		unsigned char addShadowToPrimaryLight : 1; // 0x2
		unsigned char isFoliageRequiresGroundLighting : 1; // 0x4
		unsigned char unk4 : 1; // 0x8
		unsigned char unk5 : 1; // 0x10
		unsigned char unk6 : 1; // 0x20
		unsigned char unk7 : 1; // 0x40
		unsigned char unkCastShadowMaybe : 1; // 0x80
	};

	union MaterialGameFlags
	{
		MaterialGameFlagsFields fields;
		unsigned char packed;
	};

	struct water_t
	{
		IW3::WaterWritable writable;
		IW3::complex_s* H0;
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

	union MaterialTextureDefInfo
	{
		GfxImage* image;
		water_t* water;
	};


	struct MaterialTextureDef
	{
		unsigned int nameHash;
		char nameStart;
		char nameEnd;
		unsigned char samplerState; // SamplerStateBits_e
		unsigned char semantic; // TextureSemantic
		MaterialTextureDefInfo u;
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

	/* 227 */
	enum MaterialStreamRoutingDestination : unsigned char
	{
		STREAM_DST_POSITION = 0x0,
		STREAM_DST_NORMAL = 0x1,
		STREAM_DST_COLOR_0 = 0x2,
		STREAM_DST_COLOR_1 = 0x3,
		STREAM_DST_DEPTH = 0x4,
		STREAM_DST_TEXCOORD_0 = 0x5,
		STREAM_DST_TEXCOORD_1 = 0x6,
		STREAM_DST_TEXCOORD_2 = 0x7,
		STREAM_DST_TEXCOORD_3 = 0x8,
		STREAM_DST_TEXCOORD_4 = 0x9,
		STREAM_DST_TEXCOORD_5 = 0xA,
		STREAM_DST_TEXCOORD_6 = 0xB,
		STREAM_DST_TEXCOORD_7 = 0xC,
		STREAM_DST_COUNT = 0xD,
	};


	struct MaterialStreamRouting
	{
		MaterialStreamRoutingSource source;
		MaterialStreamRoutingDestination dest;
	};

	struct MaterialVertexStreamRouting
	{
		MaterialStreamRouting data[13];
		void* decl[16];
	};

	struct MaterialVertexDeclaration
	{
		const char* name;
		char streamCount;
		bool hasOptionalSource;
		MaterialVertexStreamRouting routing;
	};

	struct MaterialTechniqueSet
	{
		const char* name;
		char worldVertFormat;
		bool hasBeenUploaded;
		char unused[1];
		MaterialTechniqueSet* remappedTechniqueSet;
		IW3::MaterialTechnique* techniques[48];
	};

#pragma pack(push, 4)
	struct Material
	{
		const char* name;
		MaterialGameFlags gameFlags;
		char sortKey;
		char textureAtlasRowCount;
		char textureAtlasColumnCount;
		IW4::GfxDrawSurf drawSurf;
		int surfaceTypeBits;
		unsigned __int16 hashIndex;
		unsigned __int16 pad;
		char stateBitsEntry[48];
		char textureCount;
		char constantCount;
		char stateBitsCount;
		StateFlags stateFlags;
		char cameraRegion;
		MaterialTechniqueSet* techniqueSet;
		MaterialTextureDef* textureTable;
		IW3::MaterialConstantDef* constantTable;
		IW3::GfxStateBits* stateBitTable;
	};
#pragma pack(pop)


	struct FxTrailDef
	{
		int scrollTimeMsec;
		int repeatDist;
		float invSplitDist;
		float invSplitArcDist;
		float invSplitTime;
		int vertCount;
		IW3::FxTrailVertex* verts;
		int indCount;
		unsigned __int16* inds;
	};

	enum ShaderCodeConstants
	{
		CONST_SRC_CODE_MAYBE_DIRTY_PS_BEGIN = 0x0,
		CONST_SRC_CODE_LIGHT_POSITION = 0x0,
		CONST_SRC_CODE_LIGHT_DIFFUSE = 0x1,
		CONST_SRC_CODE_LIGHT_SPECULAR = 0x2,
		CONST_SRC_CODE_LIGHT_SPOTDIR = 0x3,
		CONST_SRC_CODE_LIGHT_SPOTFACTORS = 0x4,
		CONST_SRC_CODE_LIGHT_FALLOFF_PLACEMENT = 0x5,
		CONST_SRC_CODE_PARTICLE_CLOUD_COLOR = 0x6,
		CONST_SRC_CODE_GAMETIME = 0x7,
		CONST_SRC_CODE_MAYBE_DIRTY_PS_END = 0x8,
		CONST_SRC_CODE_ALWAYS_DIRTY_PS_BEGIN = 0x8,
		CONST_SRC_CODE_PIXEL_COST_FRACS = 0x8,
		CONST_SRC_CODE_PIXEL_COST_DECODE = 0x9,
		CONST_SRC_CODE_FILTER_TAP_0 = 0xA,
		CONST_SRC_CODE_FILTER_TAP_1 = 0xB,
		CONST_SRC_CODE_FILTER_TAP_2 = 0xC,
		CONST_SRC_CODE_FILTER_TAP_3 = 0xD,
		CONST_SRC_CODE_FILTER_TAP_4 = 0xE,
		CONST_SRC_CODE_FILTER_TAP_5 = 0xF,
		CONST_SRC_CODE_FILTER_TAP_6 = 0x10,
		CONST_SRC_CODE_FILTER_TAP_7 = 0x11,
		CONST_SRC_CODE_COLOR_MATRIX_R = 0x12,
		CONST_SRC_CODE_COLOR_MATRIX_G = 0x13,
		CONST_SRC_CODE_COLOR_MATRIX_B = 0x14,
		CONST_SRC_CODE_SHADOWMAP_POLYGON_OFFSET = 0x15,
		CONST_SRC_CODE_RENDER_TARGET_SIZE = 0x16,
		CONST_SRC_CODE_ALWAYS_DIRTY_PS_END = 0x17,
		CONST_SRC_CODE_FIXED_PS_BEGIN = 0x17,
		CONST_SRC_CODE_DOF_EQUATION_VIEWMODEL_AND_FAR_BLUR = 0x17,
		CONST_SRC_CODE_DOF_EQUATION_SCENE = 0x18,
		CONST_SRC_CODE_DOF_LERP_SCALE = 0x19,
		CONST_SRC_CODE_DOF_LERP_BIAS = 0x1A,
		CONST_SRC_CODE_DOF_ROW_DELTA = 0x1B,
		CONST_SRC_CODE_MOTION_MATRIX_X = 0x1C,
		CONST_SRC_CODE_MOTION_MATRIX_Y = 0x1D,
		CONST_SRC_CODE_MOTION_MATRIX_W = 0x1E,
		CONST_SRC_CODE_SHADOWMAP_SWITCH_PARTITION = 0x1F,
		CONST_SRC_CODE_SHADOWMAP_SCALE = 0x20,
		CONST_SRC_CODE_ZNEAR = 0x21,
		CONST_SRC_CODE_LIGHTING_LOOKUP_SCALE = 0x22,
		CONST_SRC_CODE_DEBUG_BUMPMAP = 0x23,
		CONST_SRC_CODE_MATERIAL_COLOR = 0x24,
		CONST_SRC_CODE_FOG = 0x25,
		CONST_SRC_CODE_FOG_COLOR_LINEAR = 0x26,
		CONST_SRC_CODE_FOG_COLOR_GAMMA = 0x27,
		CONST_SRC_CODE_FOG_SUN_CONSTS = 0x28,
		CONST_SRC_CODE_FOG_SUN_COLOR_LINEAR = 0x29,
		CONST_SRC_CODE_FOG_SUN_COLOR_GAMMA = 0x2A,
		CONST_SRC_CODE_FOG_SUN_DIR = 0x2B,
		CONST_SRC_CODE_GLOW_SETUP = 0x2C,
		CONST_SRC_CODE_GLOW_APPLY = 0x2D,
		CONST_SRC_CODE_COLOR_BIAS = 0x2E,
		CONST_SRC_CODE_COLOR_TINT_BASE = 0x2F,
		CONST_SRC_CODE_COLOR_TINT_DELTA = 0x30,
		CONST_SRC_CODE_COLOR_TINT_QUADRATIC_DELTA = 0x31,
		CONST_SRC_CODE_OUTDOOR_FEATHER_PARMS = 0x32,
		CONST_SRC_CODE_ENVMAP_PARMS = 0x33,
		CONST_SRC_CODE_SUN_SHADOWMAP_PIXEL_ADJUST = 0x34,
		CONST_SRC_CODE_SPOT_SHADOWMAP_PIXEL_ADJUST = 0x35,
		CONST_SRC_CODE_COMPOSITE_FX_DISTORTION = 0x36,
		CONST_SRC_CODE_POSTFX_FADE_EFFECT = 0x37,
		CONST_SRC_CODE_VIEWPORT_DIMENSIONS = 0x38,
		CONST_SRC_CODE_FRAMEBUFFER_READ = 0x39,
		CONST_SRC_CODE_FIXED_PS_END = 0x3A,
		CONST_SRC_CODE_NON_PS_BEGIN = 0x3A,
		CONST_SRC_CODE_BASE_LIGHTING_COORDS = 0x3A,
		CONST_SRC_CODE_LIGHT_PROBE_AMBIENT = 0x3B,
		CONST_SRC_CODE_NEARPLANE_ORG = 0x3C,
		CONST_SRC_CODE_NEARPLANE_DX = 0x3D,
		CONST_SRC_CODE_NEARPLANE_DY = 0x3E,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_SCALE = 0x3F,
		CONST_SRC_CODE_CLIP_SPACE_LOOKUP_OFFSET = 0x40,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX0 = 0x41,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX1 = 0x42,
		CONST_SRC_CODE_PARTICLE_CLOUD_MATRIX2 = 0x43,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR0 = 0x44,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR1 = 0x45,
		CONST_SRC_CODE_PARTICLE_CLOUD_SPARK_COLOR2 = 0x46,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM0 = 0x47,
		CONST_SRC_CODE_PARTICLE_FOUNTAIN_PARM1 = 0x48,
		CONST_SRC_CODE_DEPTH_FROM_CLIP = 0x49,
		CONST_SRC_CODE_CODE_MESH_ARG_0 = 0x4A,
		CONST_SRC_CODE_CODE_MESH_ARG_1 = 0x4B,
		CONST_SRC_CODE_CODE_MESH_ARG_LAST = 0x4B,
		CONST_SRC_CODE_NON_PS_END = 0x4C,
		CONST_SRC_CODE_COUNT_FLOAT4 = 0x4C,
		CONST_SRC_FIRST_CODE_MATRIX = 0x4C,
		CONST_SRC_CODE_VIEW_MATRIX = 0x4C,
		CONST_SRC_CODE_INVERSE_VIEW_MATRIX = 0x4D,
		CONST_SRC_CODE_TRANSPOSE_VIEW_MATRIX = 0x4E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_MATRIX = 0x4F,
		CONST_SRC_CODE_PROJECTION_MATRIX = 0x50,
		CONST_SRC_CODE_INVERSE_PROJECTION_MATRIX = 0x51,
		CONST_SRC_CODE_TRANSPOSE_PROJECTION_MATRIX = 0x52,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_PROJECTION_MATRIX = 0x53,
		CONST_SRC_CODE_VIEW_PROJECTION_MATRIX = 0x54,
		CONST_SRC_CODE_INVERSE_VIEW_PROJECTION_MATRIX = 0x55,
		CONST_SRC_CODE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x56,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_VIEW_PROJECTION_MATRIX = 0x57,
		CONST_SRC_CODE_SHADOW_LOOKUP_MATRIX = 0x58,
		CONST_SRC_CODE_INVERSE_SHADOW_LOOKUP_MATRIX = 0x59,
		CONST_SRC_CODE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x5A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_SHADOW_LOOKUP_MATRIX = 0x5B,
		CONST_SRC_CODE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5C,
		CONST_SRC_CODE_INVERSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_OUTDOOR_LOOKUP_MATRIX = 0x5F,
		CONST_SRC_CODE_WORLD_MATRIX0 = 0x60,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX0 = 0x61,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX0 = 0x62,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX0 = 0x63,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX0 = 0x64,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX0 = 0x65,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX0 = 0x66,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX0 = 0x67,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x68,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x69,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x6A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX0 = 0x6B,
		CONST_SRC_CODE_WORLD_MATRIX1 = 0x6C,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX1 = 0x6D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX1 = 0x6E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX1 = 0x6F,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX1 = 0x70,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX1 = 0x71,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX1 = 0x72,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX1 = 0x73,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x74,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x75,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x76,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX1 = 0x77,
		CONST_SRC_CODE_WORLD_MATRIX2 = 0x78,
		CONST_SRC_CODE_INVERSE_WORLD_MATRIX2 = 0x79,
		CONST_SRC_CODE_TRANSPOSE_WORLD_MATRIX2 = 0x7A,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_MATRIX2 = 0x7B,
		CONST_SRC_CODE_WORLD_VIEW_MATRIX2 = 0x7C,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_MATRIX2 = 0x7D,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_MATRIX2 = 0x7E,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX2 = 0x7F,
		CONST_SRC_CODE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x80,
		CONST_SRC_CODE_INVERSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x81,
		CONST_SRC_CODE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x82,
		CONST_SRC_CODE_INVERSE_TRANSPOSE_WORLD_VIEW_PROJECTION_MATRIX2 = 0x83,
		CONST_SRC_TOTAL_COUNT = 0x84,
		CONST_SRC_NONE = 0x85,
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
			unsigned int randomLooping : 1;
			unsigned int shape : 1;
			unsigned int type : 2;
			unsigned int channel : 6;
		};
		unsigned int intValue;
	};

	struct snd_alias_t
	{
		const char* aliasName;
		const char* subtitle;
		const char* secondaryAliasName;
		const char* chainAliasName;
		const char* mixerGroup;
		Game::IW3::SoundFile* soundFile;
		int sequence;
		float volMin;
		float volMax;
		float pitchMin;
		float pitchMax;
		float distMin;
		float distMax;
		float velocityMin;
		SoundAliasFlags flags;
		float slavePercentage;
		float probability;
		float lfePercentage;
		float centerPercentage;
		int startDelay;
		Game::IW3::SndCurve* volumeFalloffCurve;
		float envelopMin;
		float envelopMax;
		float envelopPercentage;
		Game::IW3::SpeakerMap* speakerMap;
	};

	struct snd_alias_list_t
	{
		const char* aliasName;
		snd_alias_t* head;
		unsigned int count;
	};

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
		bool tempDefaultToCylinder;
		bool perSurfaceSndAlias;
	};


	struct Stage
	{
		const char* name;
		float origin[3];
		unsigned short triggerIndex;
		unsigned char sunPrimaryLightIndex;
	};

	struct MapEnts
	{
		const char* name;
		char* entityString;
		int numEntityChars;
		MapTriggers trigger;
		Stage* stages;
		char stageCount;
	};

	struct GfxLightImage
	{
		GfxImage* image;
		unsigned char samplerState;
	};

	struct GfxLightDef
	{
		const char* name;
		GfxLightImage attenuation;
		int lmapLookupStart;
	};

	struct RawFile
	{
		const char* name;
		unsigned int compressedLen;
		unsigned int len;
		char* buffer;
	};

	union XAssetHeader
	{
		void* data;
		PhysPreset* physPreset;
		PhysCollmap* physCollmap;
		XAnimParts* parts;
		XModelSurfs* modelSurfs;
		XModel* model;
		Material* material;
		IW3::MaterialPixelShader* pixelShader;
		IW3::MaterialVertexShader* vertexShader;
		MaterialVertexDeclaration* vertexDecl;
		MaterialTechniqueSet* techniqueSet;
		GfxImage* image;
		snd_alias_list_t* sound;
		IW3::SndCurve* sndCurve;
		IW3::LoadedSound* loadSnd;
		clipMap_t* clipMap;
		IW3::ComWorld* comWorld;
		//GameWorldSp* gameWorldSp;
		GameWorldMp* gameWorldMp;
		MapEnts* mapEnts;
		//FxWorld* fxWorld;
		GfxWorld* gfxWorld;
		GfxLightDef* lightDef;
		//Font_s* font;
		//MenuList* menuList;
		//menuDef_t* menu;
		//LocalizeEntry* localize;
		//WeaponCompleteDef* weapon;
		//SndDriverGlobals* sndDriverGlobals;
		FxEffectDef* fx;
		//FxImpactTable* impactFx;
		RawFile* rawfile;
		//StringTable* stringTable;
		//LeaderboardDef* leaderboardDef;
		//StructuredDataDefSet* structuredDataDefSet;
		//TracerDef* tracerDef;
		//VehicleDef* vehDef;
		//AddonMapEnts* addonMapEnts;
	};
}
#endif
