#pragma once

namespace Components
{
	class MapDumper : public Component
	{
	public:
		const char* getName() override { return "MapDumper"; };

		static std::string GetMapName();

		MapDumper();
		~MapDumper();

	private:
		static void DumpMap(std::string  mapName);
		static void DumpLoadedGSCs(std::string mapName);

		static std::string mapName;
	};
}
