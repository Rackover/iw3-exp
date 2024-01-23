#pragma once

namespace Components
{
	class MapDumper : public Component
	{
	public:
		const char* GetName() override { return "MapDumper"; };

		static std::string GetMapName();
		static int GetZoneIndex();
		static iw4of::api* GetApi();

		MapDumper();
		~MapDumper();

	private:
		static void DumpMap(std::string  mapName);
		static void DumpLoadedGSCs(std::string mapName, bool isSingleplayer);

		static std::string APIFileRead(const std::string& filename);

		iw4of::params_t GetParams();

		static std::string mapName;
		static int zoneIndex;
		static iw4of::api* api;
	};
}
