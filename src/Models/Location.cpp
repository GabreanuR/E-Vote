#include "../../include/Models/Location.h"
#include <algorithm>

// Region implementation
Region::Region(const json& data) {
    fromJson(data);
}

void Region::addMunicipality(const std::shared_ptr<Municipality> &municipality) {
    if (municipality && municipality->getType() == "municipality") {
        municipalities.push_back(municipality);
    }
}

void Region::removeMunicipality(int municipalityId) {
    municipalities.erase(
        std::remove_if(municipalities.begin(), municipalities.end(),
            [municipalityId](const std::shared_ptr<Location>& m) {
                return m->getId() == municipalityId;
            }),
        municipalities.end()
    );
}

bool Region::hasMunicipality(int municipalityId) const {
    return std::any_of(municipalities.begin(), municipalities.end(),
        [municipalityId](const std::shared_ptr<Location>& m) {
            return m->getId() == municipalityId;
        });
}

json Region::toJson() const {
    json data;
    data["name"] = name;
    data["municipalities"] = json::array();
    for (const auto& m : municipalities) {
        data["municipalities"].push_back(m->getId());
    }
    return data;
}

void Region::fromJson(const json& data) {
    id = data["id"].get<int>();
    name = data["name"].get<std::string>();
    // Municipalities will be populated by LocationService
}

// Municipality implementation
Municipality::Municipality(const json& data) {
    fromJson(data);
}

void Municipality::addLocality(const std::shared_ptr<Locality> &locality) {
    if (locality && locality->getType() == "locality") {
        localities.push_back(locality);
    }
}

void Municipality::removeLocality(int localityId) {
    localities.erase(
        std::remove_if(localities.begin(), localities.end(),
            [localityId](const std::shared_ptr<Location>& l) {
                return l->getId() == localityId;
            }),
        localities.end()
    );
}

bool Municipality::hasLocality(int localityId) const {
    return std::any_of(localities.begin(), localities.end(),
        [localityId](const std::shared_ptr<Location>& l) {
            return l->getId() == localityId;
        });
}

json Municipality::toJson() const {
    json data;
    data["name"] = name;
    data["regionId"] = regionId;
    data["localities"] = json::array();
    for (const auto& l : localities) {
        data["localities"].push_back(l->getId());
    }
    return data;
}

void Municipality::fromJson(const json& data) {
    id = data["id"].get<int>();
    name = data["name"].get<std::string>();
    regionId = data["region"].get<int>();
    // Localities will be populated by LocationService
}

// Locality implementation
Locality::Locality(const json& data) {
    fromJson(data);
}

json Locality::toJson() const {
    json data;
    data["id"] = id;
    data["name"] = name;
    data["municipalityId"] = municipalityId;
    return data;
}

void Locality::fromJson(const json& data) {
    id = data["id"].get<int>();
    name = data["name"].get<std::string>();
    municipalityId = data["municipality"].get<int>();
}

// NonGovernment implementation
NonGovernment::NonGovernment(const json& data) {
    fromJson(data);
}

json NonGovernment::toJson() const {
    json data;
    data["id"] = id;
    data["name"] = name;
    data["entityType"] = entityType;
    return data;
}

void NonGovernment::fromJson(const json& data) {
    id = data["id"].get<int>();
    name = data["name"].get<std::string>();
    entityType = data["entityType"].get<std::string>();
}
