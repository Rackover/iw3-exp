#include "STDInclude.hpp"

namespace Utils
{
	std::string Entities::Build()
	{
		std::string entityString;

		for (auto& entity : this->entities)
		{
			entityString.append("{\n");

			for (auto& property : entity)
			{
				entityString.push_back('"');
				entityString.append(property.first);
				entityString.append("\" \"");
				entityString.append(property.second);
				entityString.append("\"\n");
			}

			entityString.append("}\n");
		}

		return entityString;
	}

	std::vector<std::string> Entities::GetModels(bool includeDestructibles)
	{
		std::vector<std::string> models = std::vector<std::string>();
		std::ofstream destructiblesModelList;

		if (includeDestructibles) {
			const auto path = Components::GSC::GetAdditionalModelsListPath();
			destructiblesModelList.open(path);
		}

		for (auto& entity : this->entities)
		{
			if (entity.find("model") != entity.end())
			{
				std::string model = entity["model"];

				if (!model.empty()
					&& model[0] != '*' && model[0] != '?'  // Skip brushmodels
					&& model != "com_plasticcase_green_big_us_dirt"s // Skip care package (part of team zones)
					)
				{
					if (std::find(models.begin(), models.end(), model) == models.end())
					{
						models.push_back(model);
					}
				}

				if (includeDestructibles && entity.find("destructible_type") != entity.end()) {

					std::string destructible = entity["destructible_type"];

					// Then we need to fetch the destructible models
					// This is TERRIBLE but it works. Ideally we should be able to grab the destructible models from the modelpieces DynEnts list (see iGFXWorld.cpp) but it doesn't work :(
					Game::DB_EnumXAssetEntries(Game::IW3::XAssetType::ASSET_TYPE_XMODEL, [destructible, &models, &destructiblesModelList](Game::IW3::XAssetEntryPoolEntry* poolEntry)
						{
							auto entry = &poolEntry->entry;
							if (entry->inuse == 1 && entry->asset.header.model) {
								if (std::string(entry->asset.header.model->name).find(destructible) != std::string::npos) {
									std::string model = entry->asset.header.model->name;
									models.push_back(model);

									Components::Logger::Print("Saving XModel piece %s for destructible %s (from enumXAsset)\n", entry->asset.header.model->name, destructible.data());
									Components::AssetHandler::Dump(Game::IW3::XAssetType::ASSET_TYPE_XMODEL, entry->asset.header);

									(destructiblesModelList) << model << "\n";
								}
							}

						}, false);
				}
			}
		}

		if (includeDestructibles) {
			destructiblesModelList.close();
		}


		return models;
	}

	bool Entities::ConvertVehicles() {
		bool hasVehicles = false;

		for (auto& entity : this->entities)
		{
			if (entity.find("classname") != entity.end())
			{
				if (entity.find("targetname") != entity.end() &&
					entity["targetname"] == "destructible"s && 
					Utils::StartsWith(entity["destructible_type"], "vehicle"s))
				{
					entity["targetname"] = "destructible_vehicle";
					entity["sound_csv_include"] = "vehicle_car_exp";

					hasVehicles = true;
				}
			}
		}

		return hasVehicles;
	}

	void Entities::AddCarePackages()
	{
        auto subModelCount = 0;

        // We don't have a clipmap reference, so 
        Game::DB_EnumXAssetEntries(Game::IW3::XAssetType::ASSET_TYPE_CLIPMAP_PVS, [&subModelCount](Game::IW3::XAssetEntryPoolEntry* poolEntry) {
            auto entry = &poolEntry->entry;
            if (entry && entry->asset.header.clipMap && subModelCount == 0)
            {
                // A clipmap is loaded, we may add the care packages
                auto clipMap = entry->asset.header.clipMap;

                subModelCount = clipMap->numSubModels;
            }
            }, false);


        if (subModelCount > 0)
        {
            // The last two static models will always be the care packages if we added them ourselves
            auto countWithoutPackages = subModelCount - 2;

            // All values here taken from mp_rust
            std::unordered_map<std::string, std::string> airdropPalletBrushModel =
            {
                {    "script_gameobjectname",    "airdrop_pallet"},
                {    "targetname",               "iw3x_entity_carepackage"},
                {    "classname",                "script_brushmodel"},
                {    "origin",                   "-5072 6560 858"},
                {    "model",                    "*" + std::to_string(countWithoutPackages) }
            };

            std::unordered_map<std::string, std::string> airdropPalletScriptModel =
            {
                {    "ltOrigin",                 "-5072 6560.19 872.889"},
                {    "target",                   "iw3x_entity_carepackage"},
                {    "targetname",               "airdrop_crate"},
                {    "origin",                   "-5072 6560 858"},
                {    "classname",                "script_model"},
                {    "model",                    "com_plasticcase_green_big_us_dirt"}
            };

            std::unordered_map<std::string, std::string> carePackageBrushModel =
            {
                {    "script_gameobjectname",    "airdrop_pallet"},
                {    "targetname",               "iw3x_entity_airdroppallet"},
                {    "classname",                "script_brushmodel"},
                {    "origin",                   "250 325 -299"},
                {    "model",                    "*" + std::to_string(countWithoutPackages + 1) }
            };

            std::unordered_map<std::string, std::string> carePackageScriptModel =
            {
                {    "ltOrigin",                 "249.7 324.886 -299.611"},
                {    "target",                   "iw3x_entity_carepackage"},
                {    "targetname",               "care_package"},
                {    "origin",                   "249.7 324.7 -314.5"},
                {    "classname",                "script_model"},
                {    "model",                    "com_plasticcase_green_big_us_dirt"}
            };

            entities.push_back(airdropPalletBrushModel);
            entities.push_back(airdropPalletScriptModel);
            entities.push_back(carePackageBrushModel);
            entities.push_back(carePackageScriptModel);
        }
    }

	bool Entities::ConvertTurrets()
	{
		bool hasTurrets = false;

		for (auto& entity : this->entities)
		{
			if (entity.find("classname") != entity.end())
			{
				if (entity["classname"] == "misc_turret"s)
				{
					entity["weaponinfo"] = "turret_minigun_mp";
					entity["model"] = "weapon_minigun";
					hasTurrets = true;
				}
			}
		}

		return hasTurrets;
	}

	void Entities::AddRemovedSModels()
	{
		const std::string mapName = Components::MapDumper::GetMapName();
		const std::string bspName = Utils::VA("maps/mp/%s.d3dbsp", mapName.data());
		const auto header = Game::DB_FindXAssetHeader(Game::IW3::XAssetType::ASSET_TYPE_GFXWORLD, bspName.data());

		if (header.gfxWorld) {
			for (auto index : Components::IGfxWorld::removedStaticModelIndices)
			{
				auto drawInst = &header.gfxWorld->dpvs.smodelDrawInsts[index];

				Game::vec3_t angles{};
				Game::AxisToAngles(&angles, drawInst->placement.axis);
				const std::string origin = Utils::VA("%f %f %f", drawInst->placement.origin[0], drawInst->placement.origin[1], drawInst->placement.origin[2]);
				const std::string anglesStr = Utils::VA("%f %f %f", angles[0], angles[1], angles[2]);

				std::unordered_map<std::string, std::string> scriptModelEntity =
				{
                    {    "_comment",                 "added by iw3xport"},
                    {    "classname",                "script_model"},
                    {    "ltOrigin",                 origin},
                    {    "origin",                   origin},
                    {    "angles",                   anglesStr},
                    {    "model",                    drawInst->model->name }
				};

				entities.push_back(scriptModelEntity);
			}
		}
	}

	void Entities::DeleteOldSchoolPickups()
	{
		for (auto i = this->entities.begin(); i != this->entities.end();)
		{
			if (i->find("weaponinfo") != i->end() || (i->find("targetname") != i->end() && (*i)["targetname"] == "oldschool_pickup"s))
			{
				if (i->find("classname") == i->end() || (*i)["classname"] != "misc_turret"s)
				{
					Components::Logger::Print("Erased weapon %s from map ents\n", (*i)["model"].c_str());
					i = this->entities.erase(i);
					continue;
				}
			}

			++i;
		}
	}

	void Entities::parse(std::string buffer)
	{
		int parseState = 0;
		std::string key;
		std::string value;
		std::unordered_map<std::string, std::string> entity;

		for (unsigned int i = 0; i < buffer.size(); ++i)
		{
			char character = buffer[i];
			if (character == '{')
			{
				entity.clear();
			}

			switch (character)
			{
			case '{':
			{
				entity.clear();
				break;
			}

			case '}':
			{
				this->entities.push_back(entity);
				entity.clear();
				break;
			}

			case '"':
			{
				if (parseState == PARSE_AWAIT_KEY)
				{
					key.clear();
					parseState = PARSE_READ_KEY;
				}
				else if (parseState == PARSE_READ_KEY)
				{
					parseState = PARSE_AWAIT_VALUE;
				}
				else if (parseState == PARSE_AWAIT_VALUE)
				{
					value.clear();
					parseState = PARSE_READ_VALUE;
				}
				else if (parseState == PARSE_READ_VALUE)
				{
					entity[Utils::StrToLower(key)] = value;
					parseState = PARSE_AWAIT_KEY;
				}
				else
				{
					throw std::runtime_error("Parsing error!");
				}
				break;
			}

			default:
			{
				if (parseState == PARSE_READ_KEY) key.push_back(character);
				else if (parseState == PARSE_READ_VALUE) value.push_back(character);

				break;
			}
			}
		}
	}
}
