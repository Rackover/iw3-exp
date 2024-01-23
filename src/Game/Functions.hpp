#pragma once

namespace Game
{
	struct dvar_s;
	typedef void(__cdecl * Cbuf_AddText_t)(int localClientNum, const char *text);
	extern Cbuf_AddText_t Cbuf_AddText;

	typedef void(__cdecl * Cmd_ExecuteSingleCommand_t)(int controller, int a2, const char* cmd);
	extern Cmd_ExecuteSingleCommand_t Cmd_ExecuteSingleCommand;

	typedef void(__cdecl * Com_Error_t)(int type, const char* message, ...);
	extern Com_Error_t Com_Error;

	typedef void(__cdecl * Com_PrintMessage_t)(int, const char*, char);
	extern Com_PrintMessage_t Com_PrintMessage;

	typedef IW3::XAssetHeader(__cdecl * DB_FindXAssetHeader_t)(IW3::XAssetType type, const char* name);
	extern DB_FindXAssetHeader_t DB_FindXAssetHeader;

	typedef bool(__cdecl * DB_IsXAssetDefault_t)(IW3::XAssetType type, const char* name);
	extern DB_IsXAssetDefault_t DB_IsXAssetDefault;

	typedef void(__cdecl * DB_EnumXAssets_FastFile_t)(IW3::XAssetType type, void(*)(IW3::XAssetHeader, void *), void* userdata, bool overrides);
	extern DB_EnumXAssets_FastFile_t DB_EnumXAssets_FastFile;

	typedef const char* (__cdecl * DB_GetXAssetNameHandler_t)(IW3::XAssetHeader* asset);
	extern DB_GetXAssetNameHandler_t* DB_GetXAssetNameHandlers;

    typedef void(*DB_LoadXAssets_t)(XZoneInfo *zoneInfo, unsigned int zoneCount, int sync);
    extern DB_LoadXAssets_t DB_LoadXAssets;

	typedef int(*DB_GetXAssetSizeHandler_t)();
	extern DB_GetXAssetSizeHandler_t* DB_GetXAssetSizeHandlers;

	extern IW3::XAssetHeader * DB_XAssetPool;
	extern unsigned int* g_poolSize;

	typedef signed int(*FS_Read_t)(void* buffer, size_t bufferSize, int handle);
	extern FS_Read_t FS_Read;

	typedef size_t(*FS_FOpenFileRead_t)(const char* filename, int* file);
	extern FS_FOpenFileRead_t FS_FOpenFileRead;

	typedef char (*Image_LoadFromFileWithReader_t)(Game::IW3::GfxImage* a1, int(__cdecl* a2)(char*, int*));
	extern Image_LoadFromFileWithReader_t Image_LoadFromFileWithReader;

	typedef int (*FS_FOpenFileReadDatabase_t)(char* a1, int* file);
	extern FS_FOpenFileReadDatabase_t FS_FOpenFileReadDatabase;

	typedef const char* (*SEH_LocalizeTextMessage_t)(const char* a1, const char* a2, int a3);
	extern SEH_LocalizeTextMessage_t SEH_LocalizeTextMessage;

	int DB_HashForName(Game::IW3::XAssetType type, const char* name);

	Game::IW3::XAssetEntryPoolEntry* DB_FindXAssetEntry(Game::IW3::XAssetType type, const char* name);

	int FS_ReadFile(const char* path, char** buffer);
	void FS_FreeFile(void* buffer);
	void FS_FCloseFile(int handle);

	extern DWORD* cmd_id;
	extern DWORD* cmd_argc;
	extern char*** cmd_argv;
	extern cmd_function_s** cmd_ptr;

	extern IW3::XZone* g_zones;
	extern IW3::XAssetEntryPoolEntry* g_assetEntryPool;
	extern unsigned short* db_hashTable;

	extern IW3::infoParm_t* infoParams;

	void DB_EnumXAssetEntries(IW3::XAssetType type, std::function<void(IW3::XAssetEntryPoolEntry*)> callback, bool overrides);
	void Cmd_AddCommand(const char* name, void(*callback)(), cmd_function_s* data, char);
	unsigned int R_HashString(const char* string);
	const char* FindHash(unsigned int hash);
	const char* SL_ConvertToString(int idx);

	dvar_s* Dvar_FindVar(const char* dvar);

	Game::IW3::PackedUnitVec Vec3PackUnitVec(vec3_t *unitVec);
	void Vec3UnpackUnitVec(Game::IW3::PackedUnitVec in, Game::vec3_t *out);
	void AxisToAngles(Game::vec3_t* angles, Game::vec3_t* axis);
	const char* UI_SafeTranslateString(const char* locKey);

	void ConvertBounds(Game::IW4::Bounds* bounds, vec3_t mins, vec3_t maxs);
}
