#include "../include/Services/LocationService.h"
#include "../include/Utils/DataManager.h"
#include "../include/Models/User.h"
#include "../include/Utils/Types.h"
#include <algorithm>
#include <vector>
#include <memory>
#include <iostream>

LocationService* LocationService::instance = nullptr;

LocationService& LocationService::getInstance() {
    if (instance == nullptr) {
        instance = new LocationService();
        instance->loadLocations();
    }
    return *instance;
}

std::shared_ptr<Location> LocationService::createLocationFromJson(const json& data) {
    const std::string type = data["type"].get<std::string>();

    if (type == "region") {
        return std::make_shared<Region>(data);
    } if (type == "municipality") {
        return std::make_shared<Municipality>(data);
    } if (type == "locality") {
        return std::make_shared<Locality>(data);
    } if (type == "nonGovernment") {
        return std::make_shared<NonGovernment>(data);
    }
    
    return nullptr;
}

void LocationService::buildLocationHierarchy() const {
    for (auto& region : regions) {
        region->clearMunicipalities();
    }
    for (auto& municipality : municipalities) {
        municipality->clearLocalities();
    }

    for (auto& municipality : municipalities) {
        if (auto region_ptr = getRegion(municipality->getRegionId())) {
            region_ptr->addMunicipality(municipality);
        }
    }

    for (auto& locality : localities) {
        if (auto municipality_ptr = getMunicipality(locality->getMunicipalityId())) {
            municipality_ptr->addLocality(locality);
        }
    }
}

void LocationService::loadLocations() {
    json data = DataManager::loadData("data/locations.json");
    
    regions.clear();
    municipalities.clear();
    localities.clear();
    nonGovernmentEntities.clear();
    
    if (data.contains("regions")) {
        for (auto& [id_str, region_data_val] : data["regions"].items()) {
            json region_data = region_data_val;
            region_data["id"] = std::stoi(id_str);
            regions.push_back(std::make_shared<Region>(region_data));
        }
    }
    
    if (data.contains("municipalities")) {
        for (auto& [id_str, mun_data_val] : data["municipalities"].items()) {
            json mun_data = mun_data_val;
            mun_data["id"] = std::stoi(id_str);
            municipalities.push_back(std::make_shared<Municipality>(mun_data));
        }
    }
    
    if (data.contains("localities")) {
        for (auto& [id_str, loc_data_val] : data["localities"].items()) {
            json loc_data = loc_data_val;
            loc_data["id"] = std::stoi(id_str);
            localities.push_back(std::make_shared<Locality>(loc_data));
        }
    }
    
    if (data.contains("nonGovernment")) {
        for (auto& [id_str, ng_data_val] : data["nonGovernment"].items()) {
            json ng_data = ng_data_val;
            ng_data["id"] = std::stoi(id_str);
            nonGovernmentEntities.push_back(std::make_shared<NonGovernment>(ng_data));
        }
    }
    
    buildLocationHierarchy();
}

void LocationService::saveLocations() const {
    json data;

    data["regions"] = json::object();
    for (const auto& region : regions) {
        data["regions"][std::to_string(region->getId())] = region->toJson();
    }

    data["municipalities"] = json::object();
    for (const auto& municipality : municipalities) {
        data["municipalities"][std::to_string(municipality->getId())] = municipality->toJson();
    }

    data["localities"] = json::object();
    for (const auto& locality : localities) {
        data["localities"][std::to_string(locality->getId())] = locality->toJson();
    }

    data["nonGovernment"] = json::object();
    for (const auto& entity : nonGovernmentEntities) {
        data["nonGovernment"][std::to_string(entity->getId())] = entity->toJson();
    }

    DataManager::saveData("data/locations.json", data);
}

bool LocationService::addRegion(const std::string& name) {
    for (const auto& existingRegion : regions) {
        if (existingRegion->getName() == name) {
            std::cerr << "Error: A region with the name '" << name << "' already exists.\n";
            return false;
        }
    }

    int nextRegionId = 1;
    if (!regions.empty()) {
        int maxId = 0;
        for (const auto& region : regions) {
            if (region->getId() > maxId) {
                maxId = region->getId();
            }
        }
        nextRegionId = maxId + 1;
    }

    const auto region = std::make_shared<Region>(nextRegionId, name);
    regions.push_back(region);

    User::updateAdminAccess(ElectionLevel::regional, nextRegionId);
    return true;
}

bool LocationService::addMunicipality(const std::string& name, int regionId) {
    const auto parentRegion = getRegion(regionId);
    if (!parentRegion) {
        std::cerr << "Error: Parent region with ID " << regionId << " not found. Municipality not added.\n";
        return false;
    }

    for (const auto& existingMunicipality : parentRegion->getMunicipalities()) {
        if (existingMunicipality->getName() == name) {
            std::cerr << "Error: A municipality with the name '" << name << "' already exists in region '" << parentRegion->getName() << "'. \n";
            return false;
        }
    }

    int nextMunicipalityId = 1;
    if (!municipalities.empty()) {
        int maxId = 0;
        for (const auto& mun : municipalities) {
            if (mun->getId() > maxId) {
                maxId = mun->getId();
            }
        }
        nextMunicipalityId = maxId + 1;
    }

    const auto newMunicipality = std::make_shared<Municipality>(nextMunicipalityId, name, regionId);
    municipalities.push_back(newMunicipality);

    parentRegion->addMunicipality(newMunicipality);
    
    User::updateAdminAccess(ElectionLevel::municipal, nextMunicipalityId);
    return true;
}

bool LocationService::addLocality(const std::string& name, int municipalityId) {
    const auto parentMunicipality = getMunicipality(municipalityId);
    if (!parentMunicipality) {
        std::cerr << "Error: Parent municipality with ID " << municipalityId << " not found. Locality not added.\n";
        return false;
    }

    for (const auto& existingLocality : parentMunicipality->getLocalities()) {
        if (existingLocality->getName() == name) {
            std::cerr << "Error: A locality with the name '" << name << "' already exists in municipality '" << parentMunicipality->getName() << "'. \n";
            return false;
        }
    }

    int nextLocalityId = 1;
    if (!localities.empty()) {
        int maxId = 0;
        for (const auto& loc : localities) {
            if (loc->getId() > maxId) {
                maxId = loc->getId();
            }
        }
        nextLocalityId = maxId + 1;
    }

    const auto newLocality = std::make_shared<Locality>(nextLocalityId, name, municipalityId);
    localities.push_back(newLocality);

    parentMunicipality->addLocality(newLocality);
    
    User::updateAdminAccess(ElectionLevel::local, nextLocalityId);
    return true;
}

bool LocationService::addNonGovernment(const std::string& name, const std::string& entityType) {
    for (const auto& existingEntity : nonGovernmentEntities) {
        if (existingEntity->getName() == name) {
            std::cerr << "Error: A non-government entity with the name '" << name << "' already exists.\n";
            return false;
        }
    }

    int nextNonGovId = 1;
    if (!nonGovernmentEntities.empty()) {
        int maxId = 0;
        for (const auto& ng : nonGovernmentEntities) {
            if (ng->getId() > maxId) {
                maxId = ng->getId();
            }
        }
        nextNonGovId = maxId + 1;
    }

    const auto entity = std::make_shared<NonGovernment>(nextNonGovId, name, entityType);
    nonGovernmentEntities.push_back(entity);

    User::updateAdminAccess(ElectionLevel::nonGovernment, nextNonGovId);
    return true;
}

std::shared_ptr<Region> LocationService::getRegion(int id) const {
    const auto it = std::ranges::find_if(regions,
                                   [id](const std::shared_ptr<Region>& r) { return r->getId() == id; });
    return it != regions.end() ? *it : nullptr;
}

std::shared_ptr<Municipality> LocationService::getMunicipality(int id) const {
    const auto it = std::ranges::find_if(municipalities,
                                         [id](const std::shared_ptr<Municipality>& m) { return m->getId() == id; });
    return it != municipalities.end() ? *it : nullptr;
}

std::shared_ptr<Locality> LocationService::getLocality(int id) const {
    const auto it = std::ranges::find_if(localities,
                                         [id](const std::shared_ptr<Locality>& l) { return l->getId() == id; });
    return it != localities.end() ? *it : nullptr;
}

std::shared_ptr<NonGovernment> LocationService::getNonGovernment(int id) const {
    const auto it = std::ranges::find_if(nonGovernmentEntities,
                                         [id](const std::shared_ptr<NonGovernment>& n) { return n->getId() == id; });
    return it != nonGovernmentEntities.end() ? *it : nullptr;
}