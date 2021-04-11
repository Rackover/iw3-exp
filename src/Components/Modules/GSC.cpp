#include "STDInclude.hpp"

namespace Components
{
	void GSC::FixGSCFile(std::string filePath)
	{
		if (Utils::FileExists(filePath))
		{
			std::string data = Utils::ReadFile(filePath);
			GSC::ConvertGSCContents(data);
			Utils::WriteFile(filePath, data);
		}
	}

	void GSC::ConvertGSCContents(std::string& data)
	{
		Utils::Replace(data, "\r\n", "\n");
		GSC::PatchReference(data, "maps\\mp\\_utility", "common_scripts\\utility");
		GSC::PatchReference(data, "maps\\mp\\_createfx", "common_scripts\\_createfx");
		GSC::PatchReference(data, "maps\\mp\\_fx", "common_scripts\\_fx");
		data = Utils::VA("//_createfx generated. Do not touch!!\n%s", data.data());

	}

	void GSC::PatchReference(std::string& data, std::string _old, std::string _new)
	{
		// Remove absolute function paths
		Utils::Replace(data, Utils::VA("%s::", _old.data()), "");

		// Replace includes
		Utils::Replace(data, _old, _new);

		// Add our include in case it was not there
		data = Utils::VA("#include %s;\n%s", _new.data(), data.data());

		// Remove double includes (we might have created duplicates)
		auto lines = Utils::Explode(data, '\n');

		int count = 0;
		std::string newData;
		for (auto& line : lines)
		{
			if (line == Utils::VA("#include %s;", _new.data()) && count++ > 0)
			{
				continue;
			}

			newData.append(line);
			newData.append("\n");
		}

		data = newData;
	};
}
