#include "../../include/Services/LocationService.h"
#include "../../include/Utils/DataManager.h"
#include "../../include/Models/User.h"
#include "../../include/Utils/Types.h"
#include "../../include/Menus/Meniu.h"
#include <iostream>
#include <algorithm>

LocationService* LocationService::instance = nullptr;

LocationService& LocationService::getInstance() {
    if (instance == nullptr) {
        instance = new LocationService();
        instance->loadLocations(); // Load locations when instance is created
    }
    return *instance;
}

std::shared_ptr<Location> LocationService::createLocationFromJson(const json& data) {
    const std::string type = data["type"].get<std::string>();
    
    if (type == "region") {
        return std::make_shared<Region>(data);
    } else if (type == "municipality") {
        return std::make_shared<Municipality>(data);
    } else if (type == "locality") {
        return std::make_shared<Locality>(data);
    } else if (type == "nonGovernment") {
        return std::make_shared<NonGovernment>(data);
    }
    
    return nullptr;
}

void LocationService::buildLocationHierarchy() {
    // Clear existing relationships
    for (auto& region : regions) {
        region->clearMunicipalities();
    }
    for (auto& municipality : municipalities) {
        municipality->clearLocalities();
    }

    // Build region-municipality relationships
    for (auto& municipality : municipalities) {
        auto region = getRegion(municipality->getRegionId());
        if (region) {
            region->addMunicipality(municipality);
        }
    }

    // Build municipality-locality relationships
    for (auto& locality : localities) {
        auto municipality = getMunicipality(locality->getMunicipalityId());
        if (municipality) {
            municipality->addLocality(locality);
        }
    }
}

void LocationService::saveLocationHierarchy() {
    json data;
    
    // Save regions
    data["regions"] = json::object();
    for (const auto& region : regions) {
        data["regions"][std::to_string(region->getId())] = region->toJson();
    }
    
    // Save municipalities
    data["municipalities"] = json::object();
    for (const auto& municipality : municipalities) {
        data["municipalities"][std::to_string(municipality->getId())] = municipality->toJson();
    }
    
    // Save localities
    data["localities"] = json::object();
    for (const auto& locality : localities) {
        data["localities"][std::to_string(locality->getId())] = locality->toJson();
    }
    
    // Save non-government entities
    data["nonGovernment"] = json::object();
    for (const auto& entity : nonGovernmentEntities) {
        data["nonGovernment"][std::to_string(entity->getId())] = entity->toJson();
    }
    
    DataManager::getInstance().saveData("data/locations.json", data);
}

void LocationService::loadLocations() {
    json data = DataManager::getInstance().loadData("data/locations.json");
    
    regions.clear();
    municipalities.clear();
    localities.clear();
    nonGovernmentEntities.clear();
    
    if (data.contains("regions")) {
        for (auto& [id_str, region_data_val] : data["regions"].items()) {
            json region_data = region_data_val; // Make a mutable copy
            region_data["id"] = std::stoi(id_str); // Add id from key
            regions.push_back(std::make_shared<Region>(region_data));
        }
    }
    
    if (data.contains("municipalities")) {
        for (auto& [id_str, mun_data_val] : data["municipalities"].items()) {
            json mun_data = mun_data_val; // Make a mutable copy
            mun_data["id"] = std::stoi(id_str); // Add id from key
            municipalities.push_back(std::make_shared<Municipality>(mun_data));
        }
    }
    
    if (data.contains("localities")) {
        for (auto& [id_str, loc_data_val] : data["localities"].items()) {
            json loc_data = loc_data_val; // Make a mutable copy
            loc_data["id"] = std::stoi(id_str); // Add id from key
            localities.push_back(std::make_shared<Locality>(loc_data));
        }
    }
    
    if (data.contains("nonGovernment")) {
        for (auto& [id_str, ng_data_val] : data["nonGovernment"].items()) {
            json ng_data = ng_data_val; // Make a mutable copy
            ng_data["id"] = std::stoi(id_str); // Add id from key
            nonGovernmentEntities.push_back(std::make_shared<NonGovernment>(ng_data));
        }
    }
    
    buildLocationHierarchy();
}

void LocationService::saveLocations() {
    saveLocationHierarchy();
}

void LocationService::addRegion(const std::string& name) {
    // Determine next region ID from existing regions
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
    
    // Create new region
    auto region = std::make_shared<Region>(nextRegionId, name);
    regions.push_back(region);
    
    // No longer update counters.json for regionId
    
    // Save changes
    saveLocations();
    
    // Update admin access
    User::updateAdminAccess(ElectionLevel::regional, nextRegionId);
}

void LocationService::addMunicipality(const std::string& name, int regionId) {
    // Determine next municipality ID
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
    
    auto municipality = std::make_shared<Municipality>(nextMunicipalityId, name, regionId);
    municipalities.push_back(municipality);
    
    saveLocations();
    User::updateAdminAccess(ElectionLevel::municipal, nextMunicipalityId);
}

void LocationService::addLocality(const std::string& name, int municipalityId) {
    // Determine next locality ID
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
    
    auto locality = std::make_shared<Locality>(nextLocalityId, name, municipalityId);
    localities.push_back(locality);
    
    saveLocations();
    User::updateAdminAccess(ElectionLevel::local, nextLocalityId);
}

void LocationService::addNonGovernment(const std::string& name, const std::string& entityType) {
    // Determine next non-government ID
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
    
    auto entity = std::make_shared<NonGovernment>(nextNonGovId, name, entityType);
    nonGovernmentEntities.push_back(entity);
    
    saveLocations();
    User::updateAdminAccess(ElectionLevel::nonGovernment, nextNonGovId);
}

std::shared_ptr<Region> LocationService::getRegion(int id) const {
    auto it = std::find_if(regions.begin(), regions.end(),
        [id](const std::shared_ptr<Region>& r) { return r->getId() == id; });
    return it != regions.end() ? *it : nullptr;
}

std::shared_ptr<Municipality> LocationService::getMunicipality(int id) const {
    auto it = std::find_if(municipalities.begin(), municipalities.end(),
        [id](const std::shared_ptr<Municipality>& m) { return m->getId() == id; });
    return it != municipalities.end() ? *it : nullptr;
}

std::shared_ptr<Locality> LocationService::getLocality(int id) const {
    auto it = std::find_if(localities.begin(), localities.end(),
        [id](const std::shared_ptr<Locality>& l) { return l->getId() == id; });
    return it != localities.end() ? *it : nullptr;
}

std::shared_ptr<NonGovernment> LocationService::getNonGovernment(int id) const {
    auto it = std::find_if(nonGovernmentEntities.begin(), nonGovernmentEntities.end(),
        [id](const std::shared_ptr<NonGovernment>& n) { return n->getId() == id; });
    return it != nonGovernmentEntities.end() ? *it : nullptr;
}

void LocationService::displayLocations() {
    Meniu::clearScreen();
    std::cout << "===== All Locations =====\n\n";

    // Display regions and their municipalities
    std::cout << "Regions:\n";
    for (const auto& region : regions) {
        std::cout << region->getId() << ": " << region->getName() << "\n";
        std::cout << "  Municipalities: ";
        if (region->getMunicipalities().empty()) {
            std::cout << "None\n";
        } else {
            for (const auto& m : region->getMunicipalities()) {
                std::cout << m->getId() << " (" << m->getName() << ") ";
            }
            std::cout << "\n";
        }
    }

    // Display municipalities and their localities
    std::cout << "\nMunicipalities:\n";
    for (const auto& municipality : municipalities) {
        std::cout << municipality->getId() << ": " << municipality->getName() 
                  << " (Region: " << municipality->getRegionId() << ")\n";
        std::cout << "  Localities: ";
        if (municipality->getLocalities().empty()) {
            std::cout << "None\n";
        } else {
            for (const auto& l : municipality->getLocalities()) {
                std::cout << l->getId() << " (" << l->getName() << ") ";
            }
            std::cout << "\n";
        }
    }

    // Display non-government entities
    std::cout << "\nNon-Government Entities:\n";
    for (const auto& entity : nonGovernmentEntities) {
        std::cout << entity->getId() << ": " << entity->getName() 
                  << " (" << entity->getEntityType() << ")\n";
    }

    Meniu::pauseScreen();
} 