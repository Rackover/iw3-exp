#pragma once

namespace Components
{
	class GSC : public Component
	{
	public:
		const char* getName() override { return "GSC"; };

		static void FixGSCFile(std::string filePath);

	private:
		static void ConvertGSCContents(std::string& contents);
		static void PatchReference(std::string& data, std::string _old, std::string _new);
	};
}
