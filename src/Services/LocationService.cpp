#include "../include/Services/LocationService.h"
#include "../include/Utils/DataManager.h"
#include "../include/Services/UserService.h"
#include "../include/Utils/Types.h"
#include "../include/Models/Location.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <iostream>

LocationService &LocationService::getInstance() {
    static LocationService serviceInstance;
    return serviceInstance;
}

LocationService::LocationService() {
    loadLocationsFromDataManager();
}

void LocationService::buildLocationHierarchy() const {
    for (auto &region: regionsCache) {
        region->clearMunicipalities();
    }
    for (auto &municipality: municipalitiesCache) {
        municipality->clearLocalities();
    }

    for (auto &municipality: municipalitiesCache) {
        if (const auto region_ptr = getRegion(municipality->getRegionId())) {
            region_ptr->addMunicipality(municipality);
        }
    }

    for (auto &locality: localitiesCache) {
        if (const auto municipality_ptr = getMunicipality(locality->getMunicipalityId())) {
            municipality_ptr->addLocality(locality);
        }
    }
}

void LocationService::loadLocationsFromDataManager() {
    json data = DataManager::loadData(locationsFilePath);

    regionsCache.clear();
    municipalitiesCache.clear();
    localitiesCache.clear();
    nonGovernmentEntitiesCache.clear();

    if (data.is_object()) {
        if (data.contains("regions") && data["regions"].is_object()) {
            for (auto &[id_str, region_data_val]: data["regions"].items()) {
                try {
                    json region_data = region_data_val;
                    if (!region_data.contains("id")) region_data["id"] = std::stoi(id_str);
                    if (!region_data.contains("type")) region_data["type"] = "region";
                    auto region = std::make_shared<Region>(region_data);
                    regionsCache.push_back(region);
                } catch (const std::exception &e) {
                    std::cerr << "Error loading region ID " << id_str << ": " << e.what() << std::endl;
                }
            }
        }

        if (data.contains("municipalities") && data["municipalities"].is_object()) {
            for (auto &[id_str, mun_data_val]: data["municipalities"].items()) {
                try {
                    json mun_data = mun_data_val;
                    if (!mun_data.contains("id")) mun_data["id"] = std::stoi(id_str);
                    if (!mun_data.contains("type")) mun_data["type"] = "municipality";
                    auto municipality = std::make_shared<Municipality>(mun_data);
                    municipalitiesCache.push_back(municipality);
                } catch (const std::exception &e) {
                    std::cerr << "Error loading municipality ID " << id_str << ": " << e.what() << std::endl;
                }
            }
        }

        if (data.contains("localities") && data["localities"].is_object()) {
            for (auto &[id_str, loc_data_val]: data["localities"].items()) {
                try {
                    json loc_data = loc_data_val;
                    if (!loc_data.contains("id")) loc_data["id"] = std::stoi(id_str);
                    if (!loc_data.contains("type")) loc_data["type"] = "locality";
                    auto locality = std::make_shared<Locality>(loc_data);
                    localitiesCache.push_back(locality);
                } catch (const std::exception &e) {
                    std::cerr << "Error loading locality ID " << id_str << ": " << e.what() << std::endl;
                }
            }
        }

        if (data.contains("nonGovernment") && data["nonGovernment"].is_object()) {
            for (auto &[id_str, ng_data_val]: data["nonGovernment"].items()) {
                try {
                    json ng_data = ng_data_val;
                    if (!ng_data.contains("id")) ng_data["id"] = std::stoi(id_str);
                    if (!ng_data.contains("type")) ng_data["type"] = "nonGovernment";
                    auto nonGov = std::make_shared<NonGovernment>(ng_data);
                    nonGovernmentEntitiesCache.push_back(nonGov);
                } catch (const std::exception &e) {
                    std::cerr << "Error loading non-government entity ID " << id_str << ": " << e.what() << std::endl;
                }
            }
        }
    } else {
        std::cerr << "Warning: " << locationsFilePath <<
                " content is not a valid JSON object or file not found. Initializing empty lists." << std::endl;
    }

    buildLocationHierarchy();
    std::cout << "LocationService: Loaded " << regionsCache.size() << " regions, "
            << municipalitiesCache.size() << " municipalities, "
            << localitiesCache.size() << " localities, "
            << nonGovernmentEntitiesCache.size() << " non-gov entities." << std::endl;
}

void LocationService::saveLocationsToDataManager() const {
    json data;

    data["regions"] = json::object();
    for (const auto &region: regionsCache) {
        if (region) data["regions"][std::to_string(region->getId())] = region->toJson();
    }

    data["municipalities"] = json::object();
    for (const auto &municipality: municipalitiesCache) {
        if (municipality) data["municipalities"][std::to_string(municipality->getId())] = municipality->toJson();
    }

    data["localities"] = json::object();
    for (const auto &locality: localitiesCache) {
        if (locality) data["localities"][std::to_string(locality->getId())] = locality->toJson();
    }

    data["nonGovernment"] = json::object();
    for (const auto &entity: nonGovernmentEntitiesCache) {
        if (entity) data["nonGovernment"][std::to_string(entity->getId())] = entity->toJson();
    }

    DataManager::saveData(locationsFilePath, data);
    std::cout << "LocationService: Saved locations data to " << locationsFilePath << std::endl;
}

bool LocationService::addRegion(const std::string &name) {
    for (const auto &existingRegion: regionsCache) {
        if (existingRegion && existingRegion->getName() == name) {
            std::cerr << "Error: A region with the name '" << name << "' already exists.\n";
            return false;
        }
    }

    int nextRegionId = 1;
    if (!regionsCache.empty()) {
        int currentMaxId = 0;
        for (const auto &region: regionsCache) {
            if (region && region->getId() > currentMaxId) {
                currentMaxId = region->getId();
            }
        }
        nextRegionId = currentMaxId + 1;
    }

    const auto region = std::make_shared<Region>(nextRegionId, name);
    regionsCache.push_back(region);

    UserService::getInstance().grantInitialAccessToSuperAdmin(ElectionLevel::regional, nextRegionId);
    return true;
}

bool LocationService::addMunicipality(const std::string &name, int regionId) {
    const auto parentRegion = getRegion(regionId);
    if (!parentRegion) {
        std::cerr << "Error: Parent region with ID " << regionId << " not found. Municipality not added.\n";
        return false;
    }

    for (const auto &existingMunicipality: parentRegion->getMunicipalities()) {
        if (existingMunicipality && existingMunicipality->getName() == name) {
            std::cerr << "Error: A municipality with the name '" << name << "' already exists in region '" <<
                    parentRegion->getName() << "'. \n";
            return false;
        }
    }

    int nextMunicipalityId = 1;
    if (!municipalitiesCache.empty()) {
        int currentMaxId = 0;
        for (const auto &mun: municipalitiesCache) {
            if (mun && mun->getId() > currentMaxId) {
                currentMaxId = mun->getId();
            }
        }
        nextMunicipalityId = currentMaxId + 1;
    }

    const auto newMunicipality = std::make_shared<Municipality>(nextMunicipalityId, name, regionId);
    municipalitiesCache.push_back(newMunicipality);

    parentRegion->addMunicipality(newMunicipality);

    UserService::getInstance().grantInitialAccessToSuperAdmin(ElectionLevel::municipal, nextMunicipalityId);
    return true;
}

bool LocationService::addLocality(const std::string &name, int municipalityId) {
    const auto parentMunicipality = getMunicipality(municipalityId);
    if (!parentMunicipality) {
        std::cerr << "Error: Parent municipality with ID " << municipalityId << " not found. Locality not added.\n";
        return false;
    }

    for (const auto &existingLocality: parentMunicipality->getLocalities()) {
        if (existingLocality && existingLocality->getName() == name) {
            std::cerr << "Error: A locality with the name '" << name << "' already exists in municipality '" <<
                    parentMunicipality->getName() << "'. \n";
            return false;
        }
    }

    int nextLocalityId = 1;
    if (!localitiesCache.empty()) {
        int currentMaxId = 0;
        for (const auto &loc: localitiesCache) {
            if (loc && loc->getId() > currentMaxId) {
                currentMaxId = loc->getId();
            }
        }
        nextLocalityId = currentMaxId + 1;
    }

    const auto newLocality = std::make_shared<Locality>(nextLocalityId, name, municipalityId);
    localitiesCache.push_back(newLocality);

    parentMunicipality->addLocality(newLocality);

    UserService::getInstance().grantInitialAccessToSuperAdmin(ElectionLevel::local, nextLocalityId);
    return true;
}

bool LocationService::addNonGovernment(const std::string &name, const std::string &entityType) {
    for (const auto &existingEntity: nonGovernmentEntitiesCache) {
        if (existingEntity && existingEntity->getName() == name) {
            std::cerr << "Error: A non-government entity with the name '" << name << "' already exists.\n";
            return false;
        }
    }

    int nextNonGovId = 1;
    if (!nonGovernmentEntitiesCache.empty()) {
        int currentMaxId = 0;
        for (const auto &ng: nonGovernmentEntitiesCache) {
            if (ng && ng->getId() > currentMaxId) {
                currentMaxId = ng->getId();
            }
        }
        nextNonGovId = currentMaxId + 1;
    }

    const auto entity = std::make_shared<NonGovernment>(nextNonGovId, name, entityType);
    nonGovernmentEntitiesCache.push_back(entity);

    UserService::getInstance().grantInitialAccessToSuperAdmin(ElectionLevel::non_governmental, nextNonGovId);
    return true;
}

std::shared_ptr<Region> LocationService::getRegion(const int id) const {
    for (const auto &region: regionsCache) {
        if (region && region->getId() == id) {
            return region;
        }
    }
    return nullptr;
}

std::shared_ptr<Municipality> LocationService::getMunicipality(const int id) const {
    for (const auto &municipality: municipalitiesCache) {
        if (municipality && municipality->getId() == id) {
            return municipality;
        }
    }
    return nullptr;
}

std::shared_ptr<Locality> LocationService::getLocality(const int id) const {
    for (const auto &locality: localitiesCache) {
        if (locality && locality->getId() == id) {
            return locality;
        }
    }
    return nullptr;
}

std::shared_ptr<NonGovernment> LocationService::getNonGovernment(const int id) const {
    for (const auto &entity: nonGovernmentEntitiesCache) {
        if (entity && entity->getId() == id) {
            return entity;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Location> > LocationService::getAllRegionsAsBase() const {
    std::vector<std::shared_ptr<Location> > baseLocations;
    for (const auto &region: regionsCache) {
        if (region) baseLocations.push_back(region);
    }
    return baseLocations;
}

std::vector<std::shared_ptr<Location> > LocationService::getAllMunicipalitiesAsBase() const {
    std::vector<std::shared_ptr<Location> > baseLocations;
    for (const auto &municipality: municipalitiesCache) {
        if (municipality) baseLocations.push_back(municipality);
    }
    return baseLocations;
}

std::vector<std::shared_ptr<Location> > LocationService::getAllLocalitiesAsBase() const {
    std::vector<std::shared_ptr<Location> > baseLocations;
    for (const auto &locality: localitiesCache) {
        if (locality) baseLocations.push_back(locality);
    }
    return baseLocations;
}

std::vector<std::shared_ptr<Location> > LocationService::getAllNonGovernmentAsBase() const {
    std::vector<std::shared_ptr<Location> > baseNonGov;
    for (const auto &ng: nonGovernmentEntitiesCache) {
        baseNonGov.push_back(ng);
    }
    return baseNonGov;
}

std::string LocationService::getLocationName(const ElectionLevel level, const int entityId) const {
    switch (level) {
        case ElectionLevel::regional: {
            if (const auto region = getRegion(entityId)) return region->getName();
            break;
        }
        case ElectionLevel::municipal: {
            if (const auto mun = getMunicipality(entityId)) return mun->getName();
            break;
        }
        case ElectionLevel::local: {
            if (const auto loc = getLocality(entityId)) return loc->getName();
            break;
        }
        case ElectionLevel::non_governmental: {
            if (const auto ng = getNonGovernment(entityId)) return ng->getName();
            break;
        }
        case ElectionLevel::national: 
            return "National"; 
        default:
            break;
    }
    return "Unknown Entity (ID: " + std::to_string(entityId) + ")";
}

std::vector<int> LocationService::getAllLocationIds(const ElectionLevel level) const {
    std::vector<int> ids;
    switch (level) {
        case ElectionLevel::regional: {
            for (const auto &region: getAllRegions()) {
                if (region) ids.push_back(region->getId());
            }
            break;
        }
        case ElectionLevel::municipal: {
            for (const auto &mun: getAllMunicipalities()) {
                if (mun) ids.push_back(mun->getId());
            }
            break;
        }
        case ElectionLevel::local: {
            for (const auto &loc: getAllLocalities()) {
                if (loc) ids.push_back(loc->getId());
            }
            break;
        }
        case ElectionLevel::non_governmental: {
            for (const auto &ng: getAllNonGovernment()) {
                if (ng) ids.push_back(ng->getId());
            }
            break;
        }
        case ElectionLevel::national: 
            break;
        default:
            break;
    }
    return ids;
}
