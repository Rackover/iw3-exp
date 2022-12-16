#pragma once

namespace Utils
{
	class Entities
	{
	public:
		Entities() {};
		Entities(const char* string, size_t lenPlusOne) : Entities(std::string(string, lenPlusOne - 1)) {}
		Entities(std::string buffer) : Entities() { this->parse(buffer); };
		Entities(const Entities &obj) : entities(obj.entities) {};

		std::string build();

		std::vector<std::string> getModels(bool includeDestructibles);
		void deleteOldSchoolPickups();
		bool convertTurrets();
		bool convertVehicles();
		void addCarePackages();
		void addRemovedSModels();

	private:
		enum
		{
			PARSE_AWAIT_KEY,
			PARSE_READ_KEY,
			PARSE_AWAIT_VALUE,
			PARSE_READ_VALUE,
		};

		std::vector<std::unordered_map<std::string, std::string>> entities;
		void parse(std::string buffer);
	};
}
